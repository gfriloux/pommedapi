use std::path::Path;
use handlebars::{Handlebars, Helper, RenderContext, RenderError};

use pommedapi::Pommedapi;


pub struct Render {
   pub pomme: Pommedapi,
   pub hb:    Handlebars
}

impl Render {
   pub fn new(pomme: Pommedapi, template: &str) -> Result<Render, &'static str> {
      let mut hb = Handlebars::new();

      safety_on_res_return!(hb.register_template_file("html", &Path::new(template)));
      Render::render_playloads(&mut hb);

      Ok(Render {
         pomme,
         hb
      })
   }

   pub fn render_playloads(t: &mut Handlebars) {
      t.register_helper("code_to_class", Box::new(code_to_class));
      t.register_helper("disable_to_class", Box::new(disable_to_class));
   }


   pub fn run(&mut self) -> Result<String, &'static str> {
      let s = safety_on_res_return!(self.hb.render("html", &self.pomme,));
      Ok(s)
   }
}

fn disable_to_class (h: &Helper, _: &Handlebars, rc: &mut RenderContext) -> Result<(), RenderError> {
   let val = h.param(0,).and_then(|v| v.value().as_bool()).unwrap_or(false) as bool;
   let s;

   if val == true {
      s = "default";
   }
   else {
      s = "primary";
   }
   try!(rc.writer.write_all(s.as_bytes()));
   Ok(())
}

fn code_to_class(h: &Helper, _: &Handlebars, rc: &mut RenderContext) -> Result<(), RenderError> {
   let val = h.param(0,).and_then(|v| v.value().as_str(),).ok_or_else(||
      RenderError::new("late_to_warning: Param 0 with enum type is required."),
   ).unwrap();
   let s;

   s = match val {
      "Disable" => {
         "default".to_string()
      },
      "Success" => {
         "success".to_string()
      },
      "Warning" => {
         "warning".to_string()
      },
      "Danger"  => {
         "danger".to_string()
      },
      _         => {
         "default".to_string()
      }
   };

   try!(rc.writer.write_all(s.as_bytes()));
   Ok(())
}
