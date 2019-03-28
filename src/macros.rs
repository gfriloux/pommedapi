#[macro_export]
macro_rules! safety_on_res_return {
   ($code: expr) =>  {
      match $code {
         Ok(e)  => e,
         Err(e) => {
            let file   = file!();
            let line   = line!();
            println!("ERR {}:{} : Safety error : {} returns Err({})", file, line, stringify!($code), e);
            return Err("Safety check failed");
         }
      }
   }
}

#[macro_export]
macro_rules! safety_on_res_return_silent {
   ($code: expr) =>  {
      match $code {
         Ok(e)  => e,
         Err(e) => {
            let file   = file!();
            let line   = line!();
            println!("ERR {}:{} : Safety error : {} returns Err({})", file, line, stringify!($code), e);
            return;
         }
      }
   }
}
