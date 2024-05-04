// designated initializer for heap-allocated object
// compound literal will be optimized out if structure can be copied/contained
// within a register. compile-time type-checking allows for handling errors
// such as:
//
// INIT_ALLOC(struct test)
//   the assertion sizeof({__VA_ARGS__}) != sizeof(struct test) will throw
//   checks that a compound literal is passed for initialization
// INIT_ALLOC(struct test, { {1,2,3}, {4,5,6} })
//   size of compound literal must match allocated type, prevents overflows
//
// the following is a valid use of INIT_ALLOC
// INIT_ALLOC(struct test, {})
//   the macro casts the designated initializer to the type before copying,
//   this allows for null initialization of the heap-allocated object.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define __STRINGIFY(token) #token
#define __INIT_ALLOCATOR(sz) calloc(1, sz)
#define INIT_ALLOC(T, ...) ({ \
    _Static_assert(sizeof((T []){__VA_ARGS__}) == sizeof(T), \
      "Invalid designated initializer, size does not match type: " \
       __STRINGIFY(T)); \
    T* _ = __INIT_ALLOCATOR(sizeof((T []){__VA_ARGS__})); \
    _ ? (*_ = *((T []){__VA_ARGS__}), _) : NULL; \
  })

struct test {
  int x;
  int y;
  int z;
};
int main(int argc, char** argv) {
  struct test* a = INIT_ALLOC(struct test, {
    .x = 1,
    .y = 2,
    .z = 3
  });
  printf("%d %d %d\n", a->x, a->y, a->z);
  free(a);
  return 0;
}
