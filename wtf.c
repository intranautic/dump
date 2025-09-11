int main(int argc, char **argv) {
  // what am i doing ?
  struct foo {
    int a;
    int b;
  };

  struct foo obj = {.a = 1, .b = 2};
  struct foo *pobj = &obj;

  return 0;
}
