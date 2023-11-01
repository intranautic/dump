#[allow(dead_code)]

struct Human {
  name: String,
  age: u32,
}

fn main() -> Result<(), std::io::Error> {
  let x = Human {
    name: String::from("john"),
    age: 21,
  };
  drop(x);//invoke destructor
  Ok(())
}
