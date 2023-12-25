#include <stdio.h>
#include <stdlib.h>

typedef union {
  struct {
    unsigned a: 8;
    unsigned b: 8;
    unsigned c: 8;
    unsigned d: 8;
  };
  unsigned value;
} field_t;

int main(int argc, char** argv) {
  field_t x;
  x.value = 0xf0f1f2f3;
  x.a = 0;
  printf("a: %u\nb: %u\nc: %u\nd: %u\n",
    x.a, x.b, x.c, x.d);
  return 0;
}
