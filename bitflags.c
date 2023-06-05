// demo using bit flags
#include <stdio.h>
#include <stdlib.h>

enum {
  FLAG_A = 1 << 0,
  FLAG_B = 1 << 1,
  FLAG_C = 1 << 2
};

void check_flags(unsigned int flags) {
  puts((flags & FLAG_A) ? "Flag A set!" : "Flag A not set!");
  puts((flags & FLAG_B) ? "Flag B set!" : "Flag B not set!");
  puts((flags & FLAG_C) ? "Flag C set!" : "Flag C not set!");
  return;
}

int main(int argc, char** argv) {
  // flags: 00000111
  unsigned int flags = FLAG_A | FLAG_B | FLAG_C;
  check_flags(flags);
  // we can unset bitflags via bitwise and, and the not operator
  flags &= ~(FLAG_A | FLAG_B | FLAG_C);
  check_flags(flags);
  return 0;
}
