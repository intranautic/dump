// basic introduction to move semantics
#include <iostream>
#include <string>

void foo(std::string&& rv) { return; }

int main(int argc, char** argv) {
  foo("hello");
  
  std::string s = "lol";
  foo(std::move(s)); 
  foo(static_cast<std::string&&>(s));
  // error:
//  foo(s);
  return 0;
}
