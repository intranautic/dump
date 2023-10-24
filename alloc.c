#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ALLOC(type, ...) \
  (type *)memdup((type []){__VA_ARGS__}, sizeof((type []){__VA_ARGS__}))

typedef struct Object Object;
struct Object {
  int x;
  int y;
};

static void* memdup(const void* src, int size) {
  return memcpy(malloc(size), src, size);
}

void dump_object(Object* o) {
  printf("Object {\n\tx: %d\n\ty: %d\n}\n", o->x, o->y);
  return;
}

int main(int argc, char** argv) {
  Object* o = ALLOC(Object,
  {
    .x = 1,
    .y = 2
  },
  {
    .x = 3,
    .y = 4
  }
  );
  dump_object(&o[0]);
  dump_object(&o[1]);
  return 0;
}
