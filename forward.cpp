#include <iostream>
#include <string>
#include <utility>

template <typename F, typename... Args>
inline decltype(auto) invoke(F&& f, Args&&... args) {
  return std::forward<F>(f)(std::forward<Args>(args)...);
}

int foo(std::string a, int b) {
  std::cout<<a<<' '<<b<<'\n';
  return 1;
}

int main(int argc, char** argv) {
  invoke(foo, "fuckyou", 3);  
  return 0;
}
