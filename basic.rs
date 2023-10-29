// qucik and dirty hacking, adjusting to rust lol

// why is everything an expression in rust??
fn expr_fuckload() {
  // blocks are expressions???
  println!("{}", {
    println!("aaaa");
    1
  });
  // all branch structures are expressions????
  let x = true;
  println!("{}", if x {"true"} else {"false"});
  // okay, thing to note with rust syntax is that after if/else keywords
  // it specifically expects a block, not any statement. weird rigid
  // design choice ill have to look into why they did this? TODO
}

struct A {
  x: bool,
  y: i32,
}

fn main() {
  // let keyword, explicit type annotations like python?
  // by default const, let mut for mutable variables.
  let x: (char, i32) = ('a', 17);
  // lol nvim highlighting is wrong? parsing .0/.1 fields of pair as literals
  // weird..
  println!("x: {} {}", x.0, x.1);

  expr_fuckload();

  // struct literal initialization
  // assign will infer type so no need for explicit annotation
  // do it anyways for fun :)
  let a: A = A {x: true, y: 12};
  a.x; a.y; // shut the fuck up compiler

  // pattern matching, same as haskell pretty cool feature
  match a.y {
    1 => println!("opne"),
    2 => println!("two"),
    _ => println!("{}", a.y),
  }

  // okay one pretty major critique of rust's syntax is its inconsistent
  // utilization of compound blocks. struct/match utilize the same syntax
  // for block declaration yet require commas as opposed to sequences
  // of statements? this decision doesnt make sense to me..




  // fuck all this shit i want to know if i can invoke syscalls in rust
  unsafe {
    std::arch::asm!(
      "mov rax, 60",
      "mov rdi, 1337",
      "syscall"
    );
  }
  println!("failed!!");
}
