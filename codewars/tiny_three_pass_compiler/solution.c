#ifndef VECTOR
#define VECTOR

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TOMB ((void *)-1)
//#define DEBUG

struct vector {
  void** array, *front, *back;
  size_t size, index;
};
typedef struct vector vector_t;

vector_t* make_vector(size_t),
  *vector_append(vector_t*, vector_t*);
void* vector_push(vector_t*, void*),
  *vector_pop(vector_t*),
  vector_clear(vector_t*),
  *vector_insert(vector_t*, void*, size_t),
  *vector_remove(vector_t*, size_t),
  vector_resize(vector_t*, size_t),
  vector_free(vector_t*);

vector_t* make_vector(size_t size) {
  vector_t* tmp = malloc(sizeof(vector_t));
  tmp->size = size;
  tmp->index = 0;
  tmp->front = tmp->back = 0;
  tmp->array = (void **)calloc(sizeof(void *), size);
  return tmp;
}

vector_t* vector_append(vector_t* dest, vector_t* src) {
  dest->size += src->size;
  dest->array = (void **)realloc(dest->array, dest->size);
  int i, n = src->size;
  for (i = 0; i < n; ++i)
    vector_push(dest, src->array[i]);
  return dest;
}

void* vector_push(vector_t* v, void* value) {
  void** arr = v->array;
  size_t size = v->size;
  if (__glibc_unlikely(!v->front || !v->back))
    return ((arr[0] = value), (v->back = v->front = arr));
  if (__glibc_unlikely(v->index >= size))
    arr = v->array = realloc(v->array, (v->size += 100) * sizeof(long));
  return (arr[++v->index] = value, (v->front = arr[v->index]));
}

void* vector_pop(vector_t* v) {
  size_t index = v->index;
  void** arr = v->array;
  if (__glibc_unlikely(!v->front || !v->back))
    return (void *)0;
  return (index > 0)
    ? (v->front = arr[--v->index]) 
    : (v->back = v->front = 0);
}

void vector_clear(vector_t* v) {
  void** arr = v->array;
  size_t i, size = v->size;
  for (i = 0; i < size; ++i)
    arr[i] = 0;
  return;
}

void* vector_insert(vector_t* v, void* value, size_t index) {
  void** arr = v->array, *tmp;
  size_t size = v->size;
  int i, offset;
  if (index > size)
    return vector_push(v, value);
  for (i = v->size = index, offset = i, tmp = arr[i]; i < size; ++i) {
    tmp = arr[i + 1];
    arr[i + 1] = tmp;
  }
  return arr[offset] = value;
}

void* vector_remove(vector_t* v, size_t index) {
  return v->array[index] = TOMB;
}

void* vector_retrieve(vector_t* v, size_t index) {
  void** arr = v->array;
  return (arr[index] == TOMB && index < v->size)
    ? (vector_retrieve(v, index + 1))
    : (arr[index]);
}

void vector_free(vector_t* v) {
  free(v->array);
  free(v);
}

#ifdef DEBUG
void vector_test() {
  vector_t* v = make_vector(10);
  size_t i;
  for (i = 0; i < 40; ++i)
    vector_push(v, (void*)i);
  vector_insert(v, (void*)69420, 3);
  vector_remove(v, 5);
  for (i = 0; i < 40; ++i)
    printf("%d\n", vector_retrieve(v, i));
  vector_clear(v);
  vector_free(v);
}
#endif // debug
#endif // vector

#ifndef HASHMAP
#define HASHMAP

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define TOMBSTONE ((void *)1)

struct map_entry {
  char* key;
  void* value;
};
typedef struct map_entry mapentry_t;

struct hashmap {
  mapentry_t** array;
  size_t size, no_in_use;
};
typedef struct hashmap map_t;

map_t* make_map(size_t);

int map_insert(map_t*, char*, void*),
  map_resize();

void* map_retrieve(map_t*, char*);

void map_remove(map_t*, char*),
  map_free(map_t*);

unsigned long int fnv1a_hash(char*);

mapentry_t* make_mapentry(char* key, void* value) {
  mapentry_t* tmp = (mapentry_t *)malloc(sizeof(mapentry_t));
  tmp->key = key;
  tmp->value = value;
  return tmp;
}

map_t* make_map(size_t size) {
  map_t* tmp = malloc(sizeof(map_t));
  tmp->array = calloc(sizeof(mapentry_t), (tmp->size = size));
  tmp->no_in_use = 0;
  return tmp;
}

int map_insert(map_t* map, char* key, void* value) {
  unsigned long size = map->size, hash = fnv1a_hash(key) % size;

  if (map->no_in_use > size / 2)
    map_resize(map, (size = (map->size *= 2)));

  while (map->array[hash]) {
    if (__glibc_unlikely(hash >= size))
      map_resize(map, map->size * 2);
    ++hash;
  }

  map->array[hash] = make_mapentry(key, value);

  return hash;
}

// we only grow hashmap, no need to shrink
// very expensive, we really shouldn't ever need to resize unless emergency
int map_resize(map_t* map, size_t remap_size) {

  assert(remap_size > map->size);

  mapentry_t** tmparr = calloc(sizeof(mapentry_t), remap_size), *tmp;
  unsigned i, mapsize = map->size;

  for (i = 0; i < mapsize; ++i) {
    if (map->array[i]) {
      tmp = map->array[i];
      // no need to realloc, just relocate the previous entry on hashmap
      // thanks c++ move semantics lol
      if (tmp != TOMBSTONE) // ignore tombstones i guess?
        tmparr[fnv1a_hash(tmp->key) % remap_size] = map->array[i];
    }
  }

  free(map->array);
  map->array = tmparr;
  map->size = remap_size;
  return 1;
}

void* map_retrieve(map_t* map, char* key) {
  unsigned long size = map->size, hash = fnv1a_hash(key) % size;
  mapentry_t* tmp;
  while ((tmp = map->array[hash]) && hash < size) {
    if (tmp == TOMBSTONE)
      continue;
    if (!strcmp(tmp->key, key))
      return tmp->value;
  }
  return 0;
}

void map_remove(map_t* map, char* key) {
  unsigned long hash = fnv1a_hash(key);
  if (map->array[hash])
    map->array[hash] = TOMBSTONE;
  return;
}

void map_free(map_t* map) {
  unsigned i, size = map->size;
  mapentry_t* tmp;
  for (i = 0; i < size; ++i) {
    tmp = map->array[i];
    if (tmp && tmp != TOMBSTONE)
      free(map->array[i]);
  }
  free(map->array);
  free(map);
  return;
}

unsigned long int fnv1a_hash(char* key) {
  unsigned long hash = 0xcbf29ce484222325, i;
  for (i = 0; key[i] != 0; ++i) {
    hash ^= key[i];
    hash *= 0x100000001b3;
  }
  return hash;
}
#endif // hashmap

// globals :(
char** tokens;
unsigned ident_id = 0,
  id = 0, th;
map_t* global_scope; // only one identifier scope

#define CHECK_BOUNDS \
  (id < th)

/* -------------------------------------------------------------
                   main compiler porgam lol
 -------------------------------------------------------------- */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <malloc.h>

void* memdup(const void* src, size_t size) {
  return memcpy(malloc(size), src, size);
}
#define ALLOC(type, ...) \
  (type *)memdup((type []){__VA_ARGS__}, sizeof(type));

#define MATCH(x, y) \
  (x == y)

#define IS_ASCII(c) \
  (('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z'))

#define IS_INTEGER(c) \
  ('0' <= c && c <= '9')

#define ERROR_HANDLE(fmt, ...) \
  { \
    printf("\e[1;91mERROR: \e[0m" fmt, __VA_ARGS__); \
    exit(-1); \
  }

#define div divide

typedef struct AST {
  enum op {imm = 1, arg, plus = '+', min = '-', mul = '*', div = '/'} op;
  struct AST *a, *b;
  int n;
} AST;

AST* Imm (int n) {
  return ALLOC(AST, {
    .op = imm,
    .a = 0,
    .b = 0,
    .n = n
  });
}

AST* Arg (int n) {
  return ALLOC(AST, {
    .op = arg,
    .a = 0,
    .b = 0, 
    .n = n
  }); 
}

AST* Bin (enum op op, AST *a, AST *b) { 
  return ALLOC(AST, {
    .op = op,
    .a = a,
    .b = b,
    .n = 0
  });
}

// Turn a program string into an array of tokens (last entry is 0).
// Each token is either '[', ']', '(', ')', '+', '-', '*', '/', a variable
// name or a number (as a string)

char** tokenize (const char* program) {
  unsigned i = 0, sz = 32, // stream buffer counters
    j; // tmp counter
  char** stream = calloc(sizeof(char*), 32);
  const char* tmp;
  while (*program) {
    if (i + 1 >= sz) {
      stream = realloc(stream, (sz *= 2) * sizeof(char*));
      memset(stream, 0, malloc_usable_size(stream));
    }
    switch (*program) {
      case ' ':
      case '\n':
        while (*program == ' ' || *program == '\n')
          ++program;
        continue;
      case '*':
        goto unfold_op;
      case '/':
        goto unfold_op;
      case '+':
        goto unfold_op;
      case '-':
        goto unfold_op;
      case '(':
        goto unfold_op;
      case ')':
        goto unfold_op;
      case '[':
        goto unfold_op;
      case ']':
     unfold_op:
      stream[i++] = strndup(program++, 1);
      ++th;
      continue;
    }
    
    if (IS_ASCII(*program)) {
      j = 0; tmp = program;
      while (IS_ASCII(*tmp)) {
        ++tmp;
        ++j;
      }
      stream[i++] = strndup(program, j);
      ++th;
      program += j;
      continue;
    }
    
    if (IS_INTEGER(*program)) {
      j = 0; tmp = program;
      while (IS_INTEGER(*tmp)) {
        ++tmp;
        ++j;
      }
      stream[i++] = strndup(program, j);
      ++th;
      program += j;
      continue;
    }
    ERROR_HANDLE("Unkown token! %d %s:%d\n",
      *program, __FILE__, __LINE__);
  }

  return stream;
}

// Returns an un-optimized AST

void parse_arglist();

AST* parse_function(),
  *parse_expr(),
  *parse_term(),
  *parse_unary(),
  *parse_factor();

// function ::= '[' arglist ']' expr
AST* parse_function() {
  if (CHECK_BOUNDS && MATCH(*tokens[id], '[')) {
    ++id;
    parse_arglist(); // loads into global hashmap
    if (!CHECK_BOUNDS || !MATCH(*tokens[id++], ']'))
      ERROR_HANDLE("Mismatched parameter braces %s:%d\n",
        __FILE__, __LINE__);
    if (id == th)
      ERROR_HANDLE("Expected expression after paramter declaration %s:%d\n",
        __FILE__, __LINE__);
  } else
    ERROR_HANDLE("Invalid Expression, must declare function parameters %s:%d\n",
      __FILE__, __LINE__);
  return parse_expr();
}

// bad syntax, we could add a comma to act as separator for identifiers, now it depends on lexer
// arglist only loads identifiers into the map.
// no need to follow grammar, we can perform this iteratively
// arglist ::= identifier arglist
void parse_arglist() {
  global_scope = make_map(59);
  while (IS_ASCII(*tokens[id]))
    map_insert(global_scope, tokens[id++], 0); // insert api takes care of resizing
  return;
}

// expr ::= term ( '+' | '-' ) term | term
AST* parse_expr() {
  AST* result = parse_term();
  char op;
  while (CHECK_BOUNDS && (MATCH(*tokens[id], '+') || MATCH(*tokens[id], '-'))) {
    op = *tokens[id++];
    result = Bin(op, result, parse_term());
  }
  return result;
}

// term ::= unary ( '*' | '/' ) unary | unary
AST* parse_term() {
  AST* result = parse_factor();
  char op;
  while (CHECK_BOUNDS && (MATCH(*tokens[id], '*') || MATCH(*tokens[id], '/'))) {
    op = *tokens[id++];
    result = Bin(op, result, parse_factor());
  }
  return result;
}

// factor ::= identifier | integer | '(' expr ')'
AST* parse_factor() {
  char* tmp;
  AST* result; 

  if (CHECK_BOUNDS && IS_ASCII(*tokens[id])) {
    ++id;
    return Arg(ident_id++);
  }
  if (CHECK_BOUNDS && IS_INTEGER(*tokens[id]))
    return Imm(strtol(tokens[id++], &tmp, 10));

  if (CHECK_BOUNDS && MATCH(*tokens[id], '(')) {
    ++id;
    result = parse_expr();
    if (!(id < th) || !MATCH(*tokens[id++], ')'))
      ERROR_HANDLE("Mismatched Round Brackets %s:%d\n",
        __FILE__, __LINE__);
    return result;
  }
  ERROR_HANDLE("Unmatched token, unreachable has been reached! %s:%d\n",
    __FILE__, __LINE__);
  return 0;
}

AST* pass1 (const char* program) {
  tokens = tokenize (program);
  assert(tokens);
  return parse_function();
}

// Returns an AST with constant expressions reduced
AST* pass2 (AST *ast) {
  if (!ast)
    return ast;
  int lval, rval;
  if (ast->op == imm && ast->op == arg)
    return ast;
  // we could just change a/b pointers at this point?
  AST* l = pass2(ast->a),
    *r = pass2(ast->b);
  if (!r && !l)
    return ast;
  if (l->op == imm && r->op == imm) {
    // fold
    if (!ast->a || !ast->b)
      return ast;
    lval = l->n;
    rval = r->n;
    switch (ast->op) {
      case arg:
      case imm:
        return ast;
      case plus:
        return Imm(lval + rval);
      case min:
        return Imm(lval - rval);
      case mul:
        return Imm(lval * rval);
      case div:
        return Imm((lval && rval) ? (lval/rval) : (0));
      default:
        ERROR_HANDLE("Unknown Operator %s:%d\n",
          __FILE__, __LINE__);
    }
  }
  return Bin(ast->op, l, r); // left/right not constants
}

// Returns assembly instructions

static inline int n_dig(int n) {
  int c = 0;
  while (n != 0) {
    n = n / 10;
    ++c;
  }
  return c;
}

static inline char* fmt_arg(int n) {
  char* tmp = malloc(5 + n_dig(n));
  sprintf(tmp, "AR %d", n);
  return tmp;
}

static inline char* fmt_imm(int n) {
  char* tmp = malloc(5 + n_dig(n));
  sprintf(tmp, "IM %d", n);
  return tmp;
}

// wrapper for pass3
void generate(AST* node, vector_t* instructions) {
  if (!node)
    return;
  if (node->op == imm)
    vector_push(instructions, fmt_imm(node->n));
  else if (node->op == arg)
    vector_push(instructions, fmt_arg(node->n));
  else {
    generate(node->a, instructions);
    generate(node->b, instructions);
    vector_push(instructions, "PO");
    vector_push(instructions, "SW");
    vector_push(instructions, "PO");
    switch (node->op) {
      case plus:
        vector_push(instructions, "AD");
        break;
      case min:
        vector_push(instructions, "SU");
        break;
      case mul:
        vector_push(instructions, "MU");
        break;
      case div:
        vector_push(instructions, "DI");
        break;
    }
  }
  vector_push(instructions, "PU"); // push result of binary expression onto stack
  return;
}

char** pass3 (AST *ast) {
  vector_t* instructions = make_vector(50);
  generate(ast, instructions);
  vector_push(instructions, "PO"); // result should be in r0

  // array of character pointers (instructions)
  return (char **)instructions->array;
}

char** compile (const char* program) {
  // memleaks galore
  return pass3 (pass2 (pass1 (program)));
}

