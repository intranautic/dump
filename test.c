int main(int argc, char** argv) {
  if (argv[1])
    if (argv[7])
      puts("L");
    else if (argv[8])
      puts("J");
    puts("K");

  if (argv[2] != 10) {
    puts("A");
    if (argv[3]) {
      puts("B");
      while (argv[6]) {
        if (argv[4]) {
          puts("E");
          if (argv[5])
            puts("F");
        }
      }
    } else
      puts("C");
    puts("D");
  }
  return 0;
}
