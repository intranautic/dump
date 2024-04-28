// generic intrusive list pattern stolen from linux kernel
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LIST_INIT()

typedef struct list list_t;
struct list {
  struct list* prev;
  struct list* next;
};




