#![allow(unused)]

fn main() -> Result<(), std::io::Error> {
  let weekday: [&str; 5] = [
    "monday",
    "tuesday",
    "wednesday",
    "thursday",
    "friday",
  ];
  // zero out arr, infer type
  let mut z = [0; 10];
  for i in z.iter_mut() {
    *i = 1;
    println!("{}", i);
  }


  Ok(())
}
