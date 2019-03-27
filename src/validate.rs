use std::fs::File;
use std::io::Write;
use std::process::Command;
use tempfile::NamedTempFile;

use query::Query;

pub enum ValidationCode {
   Disable,
   Success,
   Warning,
   Danger
}

pub struct Validation {
   pub http_code: ValidationCode,
   pub latency:   ValidationCode,
   pub data:      ValidationCode
}


impl Validation {
   pub fn validate_code_to_string(code: &ValidationCode) -> String {
      match code {
         ValidationCode::Disable => { "Disable".to_string() },
         ValidationCode::Success => { "Success".to_string() },
         ValidationCode::Warning => { "Warning".to_string() },
         ValidationCode::Danger  => { "Danger".to_string()  }
      }
   }

   pub fn validate_latency(query: &Query) -> Result<ValidationCode, &'static str> {
      if query.disabled.unwrap_or(false) == true {
         return Ok(ValidationCode::Disable);
      }

      if query.expect.time == 0 {
         return Ok(ValidationCode::Success);
      }

      if let Some(ref x) = query.result {
         if query.expect.time > x.time {
            return Ok(ValidationCode::Success);
         }
      }

      Ok(ValidationCode::Warning)
   }

   pub fn validate_httpcode(query: &Query) -> Result<ValidationCode, &'static str> {
      if query.disabled.unwrap_or(false) == true {
         return Ok(ValidationCode::Disable);
      }

      if query.expect.http_code == 0 {
         return Ok(ValidationCode::Success);
      }

      if let Some(ref x) = query.result {
         if x.http_code == query.expect.http_code {
            return Ok(ValidationCode::Success);
         }

         if x.http_code > 200 && x.http_code < 400 {
            return Ok(ValidationCode::Warning);
         }
      }

      Ok(ValidationCode::Danger)
   }

   pub fn validate_script(query: &Query) -> Result<ValidationCode, &'static str> {
      let     file     = NamedTempFile::new().unwrap();
      let     filename = format!("{}", file.path().display());
      let     command  = format!("{}/validate", query.base_dir);
      let     output;
      /*
      let     stdout;
      let     stderr;
       */

      if query.disabled.unwrap_or(false) == true {
         return Ok(ValidationCode::Disable);
      }

      let mut f = File::create(&filename).unwrap();

      if let Some(ref x) = query.result{
         f.write_all(x.data.as_bytes()).unwrap();
         f.sync_all().unwrap();
      }
      else {
         return Ok(ValidationCode::Disable);
      }

      //println!("Executing {} to validate file {}", command, filename);

      output = Command::new(command).arg(filename).output().expect("Failed to execute process");

      /*
      stdout = String::from_utf8(output.stdout).unwrap();
      stderr = String::from_utf8(output.stderr).unwrap();
      println!("stdout:\n{}", stdout);
      println!("stderr:\n{}", stderr);
       */

      if ! output.status.success() {
         return Ok(ValidationCode::Danger);
      }

      Ok(ValidationCode::Success)
   }

   pub fn validate(query: &Query) -> Result<Validation, &'static str> {
      let data      = Validation::validate_script(query).unwrap();
      let latency   = Validation::validate_latency(query).unwrap();
      let http_code = Validation::validate_httpcode(query).unwrap();

      Ok(Validation {
         data,
         latency,
         http_code
      })
   }
}

impl std::fmt::Display for Validation {
   fn fmt(&self, f: &mut std::fmt::Formatter) -> std::fmt::Result {
      write!(f,
             "Validation http_code[{}] latency[{}] data[{}]",
             Validation::validate_code_to_string(&self.http_code),
             Validation::validate_code_to_string(&self.latency),
             Validation::validate_code_to_string(&self.data)
      )
   }
}
