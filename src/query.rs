use std::fs::File;
use std::io::{Read, Write};
use std::time::{Duration, SystemTime};
use std::process::Command;

use reqwest::ClientBuilder;
use reqwest::header::HeaderMap;
use reqwest::header::HeaderValue;
use reqwest::header::HeaderName;
use tempfile::NamedTempFile;

#[derive(Serialize, Deserialize)]
pub struct QueryHeader {
   name:        String,
   value:       String
}

#[derive(Serialize, Deserialize)]
pub struct QueryExpect {
   time:        u128,
   http_code:   u16
}

#[derive(Serialize, Deserialize)]
pub struct QueryResult {
   time:        u128,
   http_code:   u16,
   data:        String
}

#[derive(Serialize, Deserialize)]
pub struct Query {
   uri:         String,
   description: String,
   disabled:    Option<bool>,
   headers:     Vec<QueryHeader>,
   timeout:     u64,
   post_delay:  Option<u16>,
   expect:      QueryExpect,

   #[serde(skip_serializing, skip_deserializing)]
   base_uri:    String,

   query:       Option<QueryResult>,
}

impl std::fmt::Display for Query {
   fn fmt(&self, f: &mut std::fmt::Formatter) -> std::fmt::Result {
      write!(f,
             "Test:\n\turi:\t\t{}\n\tdescription:\t{}",
             self.uri, self.description)
   }
}


impl Query {
   pub fn load(file: &str, base_uri: &str) -> Result<Query, &'static str> {
      let mut data       = String::new();
      let mut obj: Query;
      let mut fd         = File::open(file).unwrap();

      fd.read_to_string(&mut data).unwrap();

      obj = serde_json::from_str(data.as_str()).unwrap();
      obj.base_uri = base_uri.to_string();
      Ok(obj)
   }

   pub fn run(&mut self) -> Result<(), &'static str> {
      let mut clientbuilder;
      let     client;
      let mut query;
      let mut headers = HeaderMap::new();
      let     now;
      let     delay;

      println!("{}", self);

      if self.disabled.unwrap_or(false) {
         return Ok(());
      }

      clientbuilder = ClientBuilder::new();
      clientbuilder = clientbuilder.timeout(Some(Duration::from_secs(self.timeout)));
      client        = clientbuilder.build().unwrap();
      query         = client.get(&format!("{}{}", self.base_uri, self.uri));

      for header in &self.headers {
         headers.insert(HeaderName::from_bytes(header.name.as_bytes()).unwrap(),
                        HeaderValue::from_str(header.value.as_str()).unwrap());
      }
      query         = query.headers(headers);

      now = SystemTime::now();
      match query.send() {
         Ok (mut e) => {
            let mut result = QueryResult {
               time: 0,
               http_code: 0,
               data: String::new()
            };
            println!("Result = {:?}", e);
            // We need to set a few results
            delay            = now.elapsed().unwrap();
            result.time      = delay.as_millis();
            result.http_code = e.status().as_u16();
            result.data      = e.text().unwrap_or("".to_string());
            self.query = Some(result);
         },
         Err(x) => {
            println!("Server return http {:?} error code", x.status());
            println!("{:?}", x);
         }
      };
      Ok(())
   }

   pub fn validate(&mut self, command: &str) -> Result<(), &'static str> {
      let     file     = NamedTempFile::new().unwrap();
      let     filename = format!("{}", file.path().display());
      let     output;
      let     stdout;
      let     stderr;

      println!("Creating file {}", filename);
      let mut f = File::create(&filename).unwrap();

      if let Some(ref x) = self.query {
         println!("Writing:\n{}", x.data);
         f.write_all(x.data.as_bytes()).unwrap();
         f.sync_all().unwrap();
      }
      else {
         return Ok(());
      }

      println!("Executing {} to validate file {}", command, filename);

      output = Command::new(command).arg(filename).output().expect("Failed to execute process");

      stdout = String::from_utf8(output.stdout).unwrap();
      stderr = String::from_utf8(output.stderr).unwrap();
      println!("stdout:\n{}", stdout);
      println!("stderr:\n{}", stderr);

      if ! output.status.success() {
         return Err("Failed to validate data");
      }

      Ok(())
   }
}
