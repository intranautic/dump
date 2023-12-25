// poc software implementation of control flow enforcement shadow stack for x86_64
// https://www.kernel.org/doc/html/next/x86/shstk.html
// gcc -fno-stack-protector -o cet cet.c 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// linked list stored on heap preserves saved return address
typedef struct _ {
  // saved return address, dont need func signature just cmp addr
  size_t ret_addr;
  struct _* next;
} shadow_entry_t;

static shadow_entry_t __g_shadow_stack;

static int shadow_call(void) {}
static int shadow_return(void) {}

void foo(void) {
  char buffer[40];
}

int main(int argc, char** argv) {

  return 0;
}
