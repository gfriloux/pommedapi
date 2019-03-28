use std::fs::File;
use std::io::Read;
use std::path::Path;
use std::time::{Duration, SystemTime};

use reqwest::ClientBuilder;
use reqwest::header::HeaderMap;
use reqwest::header::HeaderValue;
use reqwest::header::HeaderName;

use validate::Validation;

#[derive(Serialize, Deserialize)]
pub struct QueryHeader {
   name:        String,
   value:       String
}

#[derive(Serialize, Deserialize)]
pub struct QueryExpect {
   pub time:        u64,
   pub http_code:   u16
}

#[derive(Serialize, Deserialize)]
pub struct QueryResult {
   pub time:        u64,
   pub http_code:   u16,
   pub data:        String,
   pub size:        usize
}

#[derive(Serialize, Deserialize)]
pub struct Query {
   pub uri:         String,
   pub description: String,
   pub disabled:    Option<bool>,
   pub headers:     Vec<QueryHeader>,
   pub timeout:     u64,
   pub post_delay:  Option<u16>,
   pub expect:      QueryExpect,

   #[serde(skip_serializing, skip_deserializing)]
   pub base_uri:    String,

   #[serde(skip_serializing, skip_deserializing)]
   pub base_dir:    String,

   #[serde(skip_deserializing)]
   pub id:          String,

   #[serde(skip_deserializing)]
   pub data:        String,

   pub result:      Option<QueryResult>,

   pub validation:  Option<Validation>
}

impl std::fmt::Display for Query {
   fn fmt(&self, f: &mut std::fmt::Formatter) -> std::fmt::Result {
      write!(f,
             "Test:\n\turi:\t\t{}\n\tdescription:\t{}",
             self.uri, self.description)
   }
}


impl Query {
   pub fn load(id: &str, base_dir: &str, base_uri: &str) -> Result<Query, &'static str> {
      let mut data       = String::new();
      let mut obj: Query;
      let     file       = format!("{}/query.json", base_dir);
      let mut fd         = safety_on_res_return!(File::open(file));

      safety_on_res_return!(fd.read_to_string(&mut data));

      obj = safety_on_res_return!(serde_json::from_str(data.as_str()));
      obj.id       = id.to_string();
      obj.data     = "".to_string();
      obj.base_uri = base_uri.to_string();
      obj.base_dir = base_dir.to_string();
      Ok(obj)
   }

   pub fn run(&mut self) -> Result<(), &'static str> {
      let mut clientbuilder;
      let     client;
      let mut query;
      let mut headers = HeaderMap::new();
      let     now;
      let     delay;
      let     data;

      println!("{}", self);

      if self.disabled.unwrap_or(false) {
         return Ok(());
      }

      clientbuilder = ClientBuilder::new();
      clientbuilder = clientbuilder.timeout(Some(Duration::from_secs(self.timeout)));
      client        = safety_on_res_return!(clientbuilder.build());

      data = format!("{}/data", self.base_dir);

      if Path::new(&data).exists() {
         let mut fd   = safety_on_res_return!(File::open(data));
         let mut data = String::new();

         safety_on_res_return!(fd.read_to_string(&mut data));

         query      = client.post(&format!("{}{}", self.base_uri, self.uri));
         query      = query.body(data.clone());
         self.data  = data;
      }
      else {
         query      = client.get(&format!("{}{}", self.base_uri, self.uri));
      }

      for header in &self.headers {
         headers.insert(
            HeaderName::from_bytes(header.name.as_bytes()).unwrap(),
            HeaderValue::from_str(header.value.as_str()).unwrap());
      }
      query         = query.headers(headers);

      now = SystemTime::now();

      match query.send() {
         Ok (mut e) => {
            let mut result = QueryResult {
               time: 0,
               http_code: 0,
               data: String::new(),
               size: 0
            };
            //println!("Result = {:?}", e);
            // We need to set a few results
            delay            = safety_on_res_return!(now.elapsed());
            result.time      = delay.as_millis() as u64;
            result.http_code = e.status().as_u16();
            result.data      = e.text().unwrap_or("".to_string());
            result.size      = result.data.len();
            self.result = Some(result);
         },
         Err(x) => {
            println!("Server return http {:?} error code", x.status());
            println!("{:?}", x);
         }
      };
      Ok(())
   }
}
