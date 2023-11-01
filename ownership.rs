#[allow(dead_code, unused_variables)]

// playing with ownership semantics in rust

struct Human {
  name: String,
  age: i32,
}

fn main() {
  // default value assignment is move by default
  // unlike in c++ objects, moved-from objects are no longer accessible
  let x = Human { name: String::from("john"), age: 21 };
  let y = x;
  // can explicitly perform a copy

}
