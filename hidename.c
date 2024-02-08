// note: this does not actually affect the task_struct, file or inode
// the only field being effected is cmdline, which references :
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char** argv, char** envp) {
  // zero out argv, can clobber either cmdarg or environ
  // patch auxiliary vector fools ps command but not 
  memset(argv[0], 0, 16);
  memcpy(argv[0], "[fakecmd]", 8);
/*
  pid_t parent = getpid(), child;
  fork();
  // parent proc exits, child orphaned by init
  if (parent == (child = getpid()))
    return -1;

  printf("parent pid: %d\nchild pid: %d\n", parent, child);
  // child blocks
  while (!sleep(10));
*/
  getchar();
  return 0;
}
