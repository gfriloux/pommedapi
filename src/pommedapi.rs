use std::fs::{self, File};
use std::io::Read;

use config::Config;
use query::Query;
use validate::{Validation,ValidationCode};

#[derive(Serialize, Deserialize)]
pub struct PommedapiConf {
   host:        String,
   description: Option<String>
}

#[derive(Serialize, Deserialize)]
pub struct PommedapiStats {
   pub success: u16,
   pub warning: u16,
   pub danger:  u16,
   pub disable: u16
}

#[derive(Serialize, Deserialize)]
pub struct Pommedapi {
   pub param:   Config,
   pub conf:    PommedapiConf,
   pub queries: Vec<Query>,
   pub stats:   PommedapiStats
}

impl PommedapiConf {
   pub fn load(file: &str) -> Result<PommedapiConf, &'static str> {
      let mut data                = String::new();
      let     obj: PommedapiConf;
      let mut fd                  = File::open(file).unwrap();

      fd.read_to_string(&mut data).unwrap();

      obj = serde_json::from_str(data.as_str()).unwrap();
      Ok(obj)
   }
}

impl Pommedapi {
   pub fn new(param: Config) -> Pommedapi {
      Pommedapi {
         param: param.clone(),
         conf:  PommedapiConf::load(&format!("{}/pommedapi.conf", param.directory)).unwrap(),
         queries: Vec::new(),
         stats: PommedapiStats {
            success: 0,
            warning: 0,
            danger:  0,
            disable: 0
         }
      }
   }

   pub fn stat_update(&mut self, query: &Query) {
      let mut global_code = ValidationCode::Disable;

      if let Some(ref x) = query.validation {
         global_code = global_code.worst(&x.http_code);
         global_code = global_code.worst(&x.latency);
         global_code = global_code.worst(&x.data);
      }

      match global_code {
         ValidationCode::Disable => {
            self.stats.disable += 1;
         },
         ValidationCode::Success => {
            self.stats.success += 1;
         },
         ValidationCode::Warning => {
            self.stats.warning += 1;
         },
         ValidationCode::Danger  => {
            self.stats.danger  += 1;
         }
      }
   }

   pub fn run(&mut self) {
      let files = fs::read_dir(&self.param.directory).unwrap();
      let mut v: Vec<_>;

      v = files.map(|r|   r.unwrap()).collect();
      v.sort_by_key(|dir| dir.path());

      for entry in v {
         let mut query;
         let     filepath;
         let     validation;

         if ! entry.file_type().unwrap().is_dir() {
            continue;
         }

         filepath = format!("{}", entry.path().display());

         query = Query::load(entry.file_name().to_str().unwrap_or(""), &filepath, &self.conf.host).unwrap();
         query.run().unwrap();

         validation = Validation::validate(&query).unwrap();
         println!("Validation : {}", validation);

         query.validation = Some(validation);

         self.stat_update(&query);
         self.queries.push(query);
      }
   }
}
