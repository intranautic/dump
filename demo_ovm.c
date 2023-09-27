// little demonstration of an RISC ISA vm for obfuscation
// harvard architecture, code/data is split into separate regions
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct vm_context vm_context;
struct vm_context {
  struct {
    // zero flag
    unsigned char zf: 1;
    // exception flag
    unsigned char ef: 1;
    // status flag (set if currently running)
    unsigned char sf: 1;
  } flags;

  struct {
    
  } reg;

  void* code;
  void* data;
};

static vm_context* vm_create(void) {}
static vm_context* vm_load(const char* path) {}
static int vm_dump(vm_context* context, const char* name) {}
static void vm_destroy(vm_context* context) {}
static int vm_run(vm_context* context) {}


int main(int argc, char** argv) {

  return 0;
}

