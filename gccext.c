#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <asm/prctl.h>
#include <sys/syscall.h>
#include <unistd.h>

#include <sys/user.h>
#include "kpwn.h"


static inline int arch_prctl(int op, void* addr) {
  return syscall(SYS_arch_prctl, op, addr);
}

int main(int argc, char** argv) {
  // cool gcc builtin extension for compile-time retaddr introspection
  printf("main return address: %p\n\n",
      __builtin_return_address(0));

  // can manually get/set fs/gs segment register selectors
  void* __gs, *__fs;
  arch_prctl(ARCH_GET_GS, &__gs);
  arch_prctl(ARCH_GET_FS, &__fs);
  printf("retrieve fs/gs base..\ngsbase: %p\nfsbase: %p\n\n", __gs, __fs);

  void* chunk = malloc(0x200);
  arch_prctl(ARCH_SET_GS, chunk);
  arch_prctl(ARCH_GET_GS, &__gs);
  printf("setting new gs base..\nchunk: %p\nfsbase: %p\ngsbase_1: %p\n\n",
      chunk, __fs, __gs);

  getchar();
///  __seg_gs char* gs_hello = 
  return 0;
}
