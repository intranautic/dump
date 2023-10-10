// reusing moved-from objects
#include <iostream>
#include <string>
#include <vector>

int main(int argc, char** argv) {
  std::vector<std::string> v;
  std::string s;
  // when std::move is called, the moved-from object will be in a valid but
  // unspecified state. we can reuse the moved-from object and reassign
  while (std::getline(std::cin, s))
    v.push_back(std::move(s));
  return 0;
}
