// playing with thread local storage
// glibc sets fs/gs registers to tls structure
// since 8086 family 64 bit long mode only enables fs/gs segment reg how would
// the addressing scheme work? would all accesses utilize implicit segment reg
// for logical to an effective virtual address? id assume so...?
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {

  return 0;
}
