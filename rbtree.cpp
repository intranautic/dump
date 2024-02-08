#include <iostream>
#include <string>
#include <optional>

template <typename T>
struct rbnode {
  enum {
    COLOR_BLACK,
    COLOR_RED
  } color;
  struct rbnode* lhs;
  struct rbnode* rhs;
  T key;
};

template <typename T>
class rbtree {
  rbnode* _root;
public:
  // static factor method for optional construction of container
  static auto create() {}
};

int main(int argc, char** argv) {

  return 0;
}
