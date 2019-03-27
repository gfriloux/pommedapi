use std::fs::{self, File};
use std::io::Read;

use config::Config;
use query::Query;
use validate::Validation;

#[derive(Serialize, Deserialize)]
pub struct PommedapiConf {
   host:        String,
   port:        u16,
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
   param:       Config,
   conf:        PommedapiConf,

   queries:     Vec<Query>,

   stats:       PommedapiStats
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

   pub fn run(&mut self) {
      let files = fs::read_dir(&self.param.directory).unwrap();
      let mut v: Vec<_>;

      v = files.map(|r| r.unwrap()).collect();
      v.sort_by_key(|dir| dir.path());

      for entry in v {
         let mut query;
         let     filepath;
         let     validation;

         if ! entry.file_type().unwrap().is_dir() {
            continue;
         }

         filepath = format!("{}", entry.path().display());
         //println!("{}", filepath);

         query = Query::load(entry.file_name().to_str().unwrap_or(""), &filepath, &self.conf.host).unwrap();
         query.run().unwrap();


         validation = Validation::validate(&query).unwrap();
         println!("Validation : {}", validation);

         query.validation = Some(validation);

         self.queries.push(query);
      }
   }
}
