/**/

#include <stdio.h>

#ifndef BISKIT_H_
#define BISKIT_H_

// fwd decl in public header
typedef struct _biskit biskit_t;

#endif // BISKIT_H_

struct biskit {};

__attribute__((constructor((1<<16)-1)))
void biskit_init(void) {
  puts("fuck");
}

int main(int argc, char** argv) {

  return 0;
}
