#[allow(dead_code, unused_variables)]

// playing with ownership semantics in rust

#[derive(Debug, Clone)]
struct Human {
  name: String,
  age: i32,
}

fn own(x: Human) -> Human {x}
fn dontown(x: &Human) -> &Human { x}

fn main() {
  // default value assignment is move by default
  // unlike in c++ objects, moved-from objects are no longer accessible
  let mut x = Human { name: String::from("john"), age: 21 };
  let mut y = x;
  y = own(y);
  dontown(&y);
  // can explicitly perform a copy
  // stupid, clone but reassign just create new ik, only for demo
  x = y.clone();
  x.name = String::from("jake");
  x.age = 23;
  println!("{:?}\n{:?}", x, y);
}
