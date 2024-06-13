#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// imported from /dump/alloc.c
#ifndef INIT_ALLOC_H_
#define INIT_ALLOC_H_
#define __STRINGIFY(token) #token
#define __INIT_ALLOCATOR(sz) calloc(1, sz)
#define INIT_ALLOC(T, ...) ({ \
    _Static_assert(sizeof((T []){__VA_ARGS__}) == sizeof(T), \
      "Invalid designated initializer, size does not match type: " \
       __STRINGIFY(T)); \
    T* _ = __INIT_ALLOCATOR(sizeof((T []){__VA_ARGS__})); \
    _ ? (*_ = *((T []){__VA_ARGS__}), _) : NULL; \
  })
#endif // INIT_ALLOC_H_

/*
 * each leaf should contain code-source mapping index, dont need to actually
 * include bitset. during traversal build bitset on stack-allocated container,
 * max coding length should be tree height, so we can use an optimal+simple
 * static bitset container.
 *
 * equivalently calculate via shannon entropy function lower bound.
 *
 */

#define _DEBUG
#define DOMAIN(T) ((1 << (sizeof(T) * 8)) -1)
#define list_foreach(T, it, head) \
  for (T it = head; it; it = it->next)
#define list_pop(T, ctx) \
  ({T tmp = ctx; (ctx) ? (ctx = ctx->next, tmp) : 0; })

typedef unsigned char symbol_t;
typedef struct huffman_node huffman_node_t;
typedef struct huffman_tree huffman_tree_t;
typedef int freqmap_t[DOMAIN(symbol_t)];

struct huffman_node {
  symbol_t symbol; //prefix
  struct huffman_node* parent;
  struct huffman_node* children[2];
  struct huffman_node* next; //for huffman_tree_t->symbols
};

struct huffman_tree {
  freqmap_t map_freq;
  struct huffman_node* root;
  struct huffman_node* symbols; // leaves of tree
};

//gross
#define __FRQCMP(map, cmp, a, b) \
  ((map[a->symbol]) cmp (map[b->symbol]))
// construct a tree given a character stream
huffman_tree_t* huffman_create(char* stream, unsigned size) {
  huffman_tree_t* hf_tree = INIT_ALLOC(struct huffman_tree, {});
  // calculate frequency map
  for (unsigned i = 0; i < size; ++i)
    hf_tree->map_freq[stream[i]]++;

  // generate symbol leaf nodes, only alloc for symbols in freq map
  huffman_node_t* tmp = NULL;
  for (unsigned i = 0; i < DOMAIN(symbol_t); ++i) {
    if (hf_tree->map_freq[i]) {
      tmp = INIT_ALLOC(struct huffman_node, {
        .symbol = i,
        .parent = NULL,
        .children = {NULL, NULL},
        .next = tmp
      });
    }
  }
  // no symbols, no work left for us
  if (!tmp)
    return hf_tree;

  // insertion sort pass in reverse order, gross
  huffman_node_t* next, *curr;
  while (tmp) {
    next = tmp->next;
    if (!hf_tree->symbols || __FRQCMP(hf_tree->map_freq, <=, hf_tree->symbols, tmp)) {
      tmp->next = hf_tree->symbols;
      hf_tree->symbols = tmp;
    } else {
      curr = hf_tree->symbols;
      while (curr->next && __FRQCMP(hf_tree->map_freq, >, curr->next, tmp))
        curr = curr->next;
      tmp->next = curr->next;
      curr->next = tmp;
    }
    tmp = next;
  }

#ifdef _DEBUG
  puts("Frequency map debug:");
  list_foreach(huffman_node_t*, it, hf_tree->symbols)
    printf("%c: %d\n", it->symbol, hf_tree->map_freq[it->symbol]);
#endif

  // TODO: construct the prefix binary tree


  return hf_tree;
}

void huffman_destroy(huffman_tree_t* hf_tree) {
  if (hf_tree) {
    if (hf_tree->symbols) {
      huffman_node_t* prev = hf_tree->symbols, *it;
      list_foreach(, it, hf_tree->symbols->next) {
        free(prev);
        prev = it;
      }
      free(prev);
    }
    free(hf_tree);
  }
  return;
}

int main(int argc, char** argv) {
  if (!argv[1])
    return -1;
  huffman_tree_t* hf_tree = huffman_create(argv[1], strlen(argv[1]));
  huffman_destroy(hf_tree);

  return 0;
}
