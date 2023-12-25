#include <cstdio>
#include <cstdlib>
#include <cstring>

struct Resource {
  Resource(const char* src)
    : length(strlen(src))
  {
    buffer = (char *)malloc(this->length);
    if (buffer)
      strncpy(buffer, src, this->length);
  }
  char* buffer;
  int length;
};

int main(int argc, char** argv) {
  auto x = new Resource("hello there");
  printf("%s\n%d\n", x->buffer,x->length);
  return 0;
}
