#include <stdarg.h>

void foo(const char* fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  va_end(ap);
  return;
}

int main(int argc, char** argv) {
  foo("fucker", "dick");
  return 0;
}
