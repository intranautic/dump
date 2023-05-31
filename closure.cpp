#include <iostream>

auto foo(int x) {
  return ([=] (int y) -> int {
    return x + y;
  });
}

int main(int argc, char**argv) {
  auto bar = foo(10);
  std::cout<< bar(20) << std::endl;
  return 0;
}
