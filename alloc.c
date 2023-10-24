/*
general concept, utilize static_assert to both typecheck that type is compatible with object
being passed to macro. but one problem is that the macro itself casts the type so by default
equivalent. next check arguments, and find some way of enforcing type as a type, opposed to
literal and et cetera.

i give up, this is a disaster and i never want to do this again.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// debugging/helper macro wrappers
#define _TOSTR_INTERNAL(str) #str
#define _TOSTR(str) _TOSTR_INTERNAL(str)
#define _ASSERT_SLIP(x, msg) \
  sizeof(struct {_Static_assert(x, msg); char _;})
#define _ALLOC_DEBUG(msg) "_ALLOC: " msg " on line " _TOSTR(__LINE__)

#define _ALLOC_INTERNAL(type, ...) \
  ( \
    _ASSERT_SLIP(({type _;}), _ALLOC_DEBUG("Incompatible types")), \
    (type *)memdup((type[]){__VA_ARGS__}, sizeof((type []){__VA_ARGS__})) \
  )

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
  Object* o = _ALLOC_INTERNAL(1, {});
  return 0;
}
