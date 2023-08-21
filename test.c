#include <stdio.h>
#include <stdint.h>

#define FNV64_OFFSET 0xcbf29ce484222325
#define FNV64_PRIME  0x100000001b3

static size_t fnv1a_hash64(void* input, size_t size) {
  size_t hash = FNV64_OFFSET;
  for (size_t i = 0; i < size; ++i) {
    hash ^= ((uint8_t *)input)[i];
    printf("%c\n", ((uint8_t *)input)[i]);
    hash *= FNV64_PRIME;
  }
  return hash;
}

int main(int argc) {
  printf("%lu\n", fnv1a_hash64("hello", 5));
}
