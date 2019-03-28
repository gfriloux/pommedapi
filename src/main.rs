extern crate getopts;
#[macro_use] extern crate serde_derive;
extern crate serde_json;
extern crate reqwest;
extern crate tempfile;
extern crate handlebars;
extern crate fs_extra;

mod config;
mod pommedapi;
mod query;
mod validate;
mod render;

use std::fs::File;
use std::io::Write;
use getopts::Options;
use std::env;
use std::fs;
use std::path::Path;
use fs_extra::dir::CopyOptions;
use fs_extra::copy_items;

use config::Config;
use pommedapi::Pommedapi;
use render::Render;

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
   let mut render;
   let     s;

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

   pomme  = Pommedapi::new(config);
   pomme.run();

   render = Render::new(pomme, "data/templates/template.hbs").unwrap();
   s      = render.run().unwrap();

   // Save output
   println!("Checking {}", render.pomme.param.output);
   if ! Path::new(&render.pomme.param.output).exists() {
      fs::create_dir(&render.pomme.param.output).unwrap();
   }

   let datadir = format!("{}/data", render.pomme.param.output);
   if ! Path::new(&datadir).exists() {
      fs::create_dir(&datadir).unwrap();
   }

   let mut options = CopyOptions::new();
   options.overwrite = true;

   let mut files = Vec::new();

   files.push("/usr/local/share/pommedapi/css/");
   files.push("/usr/local/share/pommedapi/js");
   files.push("/usr/local/share/pommedapi/fonts");

   copy_items(&files, &datadir, &options).unwrap();

   let mut f = File::create(&format!("{}/{}",
                                     render.pomme.param.output,
                                     render.pomme.param.filename)
                           ).unwrap();
   f.write_all(s.as_bytes()).unwrap();
   f.sync_all().unwrap();
}
