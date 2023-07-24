#include <stdio.h>
#include <unistd.h>

int main(int argc, char** argv) {
  int flag = 0;
  char buffer[1024];
  scanf("%s", &buffer);
  printf(buffer);
  if (flag)
    puts("WIN");
  return 0;
}
