use std::fs::File;
use std::io::Write;
use std::path::Path;
use std::process::Command;
use tempfile::NamedTempFile;

use query::Query;

#[derive(Serialize, Deserialize, PartialEq, Clone)]
pub enum ValidationCode {
   Disable,
   Success,
   Warning,
   Danger
}

#[derive(Serialize, Deserialize)]
pub struct Validation {
   pub http_code:  ValidationCode,
   pub latency:    ValidationCode,
   pub data:       ValidationCode,
   pub output:     String
}


impl Validation {
   pub fn latency(&mut self, query: &Query) -> Result<(), &'static str> {
      if query.disabled.unwrap_or(false) == true {
         self.latency = ValidationCode::Disable;
         return Ok(());
      }

      if query.expect.time == 0 {
         self.latency = ValidationCode::Success;
         return Ok(());
      }

      if let Some(ref x) = query.result {
         if query.expect.time > x.time {
            self.latency = ValidationCode::Success;
            return Ok(());
         }
      }

      self.latency = ValidationCode::Warning;
      Ok(())
   }

   pub fn httpcode(&mut self, query: &Query) -> Result<(), &'static str> {
      if query.disabled.unwrap_or(false) == true {
         self.http_code = ValidationCode::Disable;
         return Ok(());
      }

      if query.expect.http_code == 0 {
         self.http_code = ValidationCode::Success;
         return Ok(());
      }

      if let Some(ref x) = query.result {
         if x.http_code == query.expect.http_code {
            self.http_code = ValidationCode::Success;
            return Ok(());
         }

         if x.http_code > 200 && x.http_code < 400 {
            self.http_code = ValidationCode::Warning;
            return Ok(());
         }
      }

      self.http_code = ValidationCode::Danger;
      Ok(())
   }

   pub fn script(&mut self, query: &Query) -> Result<(), &'static str> {
      let     file     = safety_on_res_return!(NamedTempFile::new());
      let     filename = format!("{}", file.path().display());
      let     command  = format!("{}/validate", query.base_dir);
      let     output;

      if query.disabled.unwrap_or(false) == true {
         self.data = ValidationCode::Disable;
         return Ok(());
      }

      if ! Path::new(&command).exists() {
         self.data = ValidationCode::Disable;
         return Ok(());
      }

      let mut f = safety_on_res_return!(File::create(&filename));

      if let Some(ref x) = query.result{
         safety_on_res_return!(f.write_all(x.data.as_bytes()));
         safety_on_res_return!(f.sync_all());
      }
      else {
         self.data = ValidationCode::Disable;
         return Ok(());
      }

      //println!("Executing {} to validate file {}", command, filename);

      output = safety_on_res_return!(Command::new(command).arg(filename).output());
      self.output = safety_on_res_return!(String::from_utf8(output.stdout));

      if ! output.status.success() {
         self.data = ValidationCode::Danger;
         return Ok(());
      }

      self.data = ValidationCode::Success;
      Ok(())
   }

   pub fn validate(query: &Query) -> Result<Validation, &'static str> {
      let mut validation = Validation {
         latency:    ValidationCode::Disable,
         http_code:  ValidationCode::Disable,
         data:       ValidationCode::Disable,
         output:     String::new()
      };

      safety_on_res_return!(validation.script(query));
      safety_on_res_return!(validation.latency(query));
      safety_on_res_return!(validation.httpcode(query));

      Ok(validation)
   }
}

impl ValidationCode {
   pub fn worst(&self, val: &ValidationCode) -> ValidationCode {
      match self {
         ValidationCode::Disable => {
            if self != val {
               return val.clone();
            }
            return ValidationCode::Disable;
         },
         ValidationCode::Success => {
            if val == &ValidationCode::Disable {
               return ValidationCode::Success;
            }
            return val.clone();
         },
         ValidationCode::Warning => {
            if val == &ValidationCode::Danger {
               return ValidationCode::Danger;
            }
            return ValidationCode::Warning;
         },
         ValidationCode::Danger  => {
            return ValidationCode::Danger;
         }
      };
   }
}

impl std::fmt::Display for Validation {
   fn fmt(&self, f: &mut std::fmt::Formatter) -> std::fmt::Result {
      write!(f,
             "Validation http_code[{}] latency[{}] data[{}]",
             self.http_code, self.latency, &self.data
      )
   }
}

impl std::fmt::Display for ValidationCode {
   fn fmt(&self, f: &mut std::fmt::Formatter) -> std::fmt::Result {
      let s;
      s = match self {
         ValidationCode::Disable => { "disable".to_string() },
         ValidationCode::Success => { "success".to_string() },
         ValidationCode::Warning => { "warning".to_string() },
         ValidationCode::Danger  => { "danger".to_string()  }
      };
      write!(f, "{}", s)
   }
}
