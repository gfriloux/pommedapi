
#[derive(Serialize, Deserialize, Clone)]
pub struct Config {
   pub directory: String,
   pub output:    String,
   pub filename:  String
}

impl Config {
   pub fn new(directory: Option<String>, output: Option<String>, filename: Option<String>) -> Config {
      Config {
         directory: directory.unwrap_or("pommedapi".to_string()),
         output:    output.unwrap_or("html".to_string()),
         filename:  filename.unwrap_or("filename".to_string())
      }
   }
}

impl std::fmt::Display for Config {
   fn fmt(&self, f: &mut std::fmt::Formatter) -> std::fmt::Result {
      write!(f,
             "Configuration:\n\tDirectory:\t{}\n\tOutput:\t\t{}\n\tFilename:\t{}\n",
             self.directory, self.output, self.filename)
   }
}
