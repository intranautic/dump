// neat little trick which can probably improve an external API for
// a framework or library. just a little hack, not much else you can
// do with this. (since c++11).

// we are allowed to overload a user defined prefix that operates on
// "integers, floating point, characters, and string literals".
#include <iostream>
using ld = long double;

ld operator "" _kb(long double b) {
  return b * 1000;
}

int main(int argc, char**argv) {
  std::cout << 1.2_kb << std::endl;
  return 0;
}
