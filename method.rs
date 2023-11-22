struct Point {
  x: i32,
  y: i32,
}

impl Point {
  fn square(&self) -> i64 {
    (self.x * self.y).into()
  }
}

fn main() -> Result<(), std::io::Error> {
  let x = Point { x: 1, y: 2 };
  println!("{}", x.square());

  Ok(())
}
