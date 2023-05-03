// experiment??
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>

#define PAGE_SIZE 0x1000

int main(int argc, char** argv) {
  void* x = mmap(0, PAGE_SIZE, PROT_READ|PROT_WRITE,
    MAP_PRIVATE|MAP_ANONYMOUS, -1, 0);
  memset(x, 'A', PAGE_SIZE);
  printf("Page x @ %#lx: %.10s...\n", x, x);
  getchar();
  void* y = mmap(x, PAGE_SIZE, PROT_READ|PROT_WRITE,
    MAP_PRIVATE|MAP_ANONYMOUS|MAP_FIXED, -1, 0);
  printf("Page x @ %#lx: %.10s...\n", x, x);
  printf("Page y @ %#lx: %.10s...\n", y, y);
  getchar();
  return 0;
}

