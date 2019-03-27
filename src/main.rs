extern crate getopts;
#[macro_use] extern crate serde_derive;
extern crate serde_json;
extern crate reqwest;
extern crate tempfile;

mod config;
mod pommedapi;
mod query;
mod validate;

use getopts::Options;
use std::env;

use config::Config;
use pommedapi::Pommedapi;

fn usage(program: &str, opts: Options) {
   let brief = format!("Usage: {} [options]", program);
   print!("{}", opts.usage(&brief));
}

fn main() {
   let     args: Vec<String> = env::args().collect();
   let mut opts              = Options::new();
   let     program           = args[0].clone();
   let     config: Config;
   let mut pomme;

   opts.optopt ("t", "testdir",  "Directory of tests to run.",                          "NAME");
   opts.optopt ("o", "output",   "Directory to use for HTML output.",                   "NAME");
   opts.optopt ("f", "filename", "Force filename of HTML file (index.html by default)", "NAME");
   opts.optflag("h", "help",     "print this help menu");

   let matches = match opts.parse(&args[1..]) {
        Ok (m) => { m                     }
        Err(f) => { panic!(f.to_string()) }
    };

   if matches.opt_present("h") {
      usage(&program, opts);
      return;
   }


   config = Config::new(matches.opt_str("testdir"),
                        matches.opt_str("output"),
                        matches.opt_str("filename"));

   println!("{}", config);

   pomme = Pommedapi::new(config);
   pomme.run();
}
