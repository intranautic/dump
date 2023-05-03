// demo using bit flags
#include <stdio.h>
#include <stdlib.h>

enum {
  FLAG_A = 1 << 0,
  FLAG_B = 1 << 1,
  FLAG_C = 1 << 2
};

int main(int argc, char** argv) {
  // flags: 00000111
  unsigned char flags = FLAG_A | FLAG_B | FLAG_C;
  if (flags & FLAG_A)
    puts("Flag A set!");

  if (flags & FLAG_B)
    puts("Flag B set!");

  if (flags & FLAG_C)
    puts("Flag C set!");

  return 0;
}
