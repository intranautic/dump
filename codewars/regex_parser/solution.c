#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct RegExp RegExp;

RegExp* any () {}
RegExp* normal (char c) {}
RegExp* zeroOrMore (RegExp *starred) {}
RegExp* or (RegExp *left, RegExp *right) {}
RegExp* str (RegExp *first) {}
RegExp* add (RegExp *str, RegExp *next) {}

RegExp *parseRegExp (char *input) {
  return 0;
}
