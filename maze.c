#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define _HEAP_ALLOCATOR(sz) calloc(1, sz)
#define _MEMDUP(src, size) memcpy(_HEAP_ALLOCATOR(size), src, size)
#define INIT_ALLOC(T, ...) \
  _MEMDUP(((T []){__VA_ARGS__}), sizeof((T []){__VA_ARGS__}))


typedef struct maze Maze;
struct maze {
  int col;
  int row;
  char *buffer;
};

void shuffle(int *array, size_t n) {
  for (size_t i = n - 1; i > 0; i--) {
    size_t j = rand() % (i + 1);
    int temp = array[i];
    array[i] = array[j];
    array[j] = temp;
  }
  return;
}


//recursive backtracking maze carve algo?
#define CHECK_BOUND(x, cx, y, cy) \
  (x > 0 && x < cx && y > 0 && y < cy)
void carve(Maze *maze, int x, int y) {
  int dx, dy;
  int nx, ny, mx, my;
  int directions[4] = {0, 1, 2, 3};
  shuffle(directions, 4);

  // stupdi fucking swithc we can just map but i cant be fucking
  // BOTH#ERRRREED
  for (int i = 0; i < 4; i++) {
    dx = 0, dy = 0;
    switch (directions[i]) {
      case 0: dx = 2; break;
      case 1: dx = -2; break;
      case 2: dy = 2; break;
      case 3: dy = -2; break;
    }
    nx = x + dx, ny = y + dy;
    mx = x + dx / 2, my = y + dy / 2;

    if (CHECK_BOUND(nx, maze->col, ny, maze->row)) {
      if (maze->buffer[ny*maze->col+ nx] == '#') {
        maze->buffer[ny * maze->col + nx] = ' ';
        maze->buffer[my * maze->col + mx] = ' ';
        carve(maze, nx, ny);
      }
    }
  }
  return;
}

int main(int argc, char** argv) {
  if (argc <= 3)
    return (printf("Usage: %s <col> <row> <entry>\n", argv[0]), 1);

  srand(time(0));
  int col =
    atoi(argv[1]),
    row = atoi(argv[2]),
    entry = atoi(argv[3]);

  Maze* maze = INIT_ALLOC(Maze, {
    .col = col,
    .row = row,
    .buffer = malloc(col*row+1)
  });
  memset(maze->buffer, '#', col*row);

  // remember entry must be odd offset, edge cannot be accessed
  carve(maze, entry, 1);


  // set entry & exit, pretty shit doesnt affect anything
  // fuckmy life fuck my LIFEEEE shit shit no guarantee of exit
  // dont want to make conditional works some percent of the time i fucking
  // hope
  maze->buffer[entry] = ' ';
  maze->buffer[(row-1)*col + (entry*2) % row] = ' ';

  for (int i = 0; i < maze->row; ++i) {
    for (int j = 0; j < maze->col; ++j)
      putchar(maze->buffer[i*maze->col + j]);
    putchar('\n');
  }

  free(maze->buffer);
  free(maze);
  return 0;
}
