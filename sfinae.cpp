// substitution failure is not an error
// we can use this technique to provide semantic constraints on template parameters, providing
// better error messages at invalid template instantiations. this allows for much better debugging
// and code maintainability. the recent c++20 concepts aims to provide stl support for sfinae,
// without the inconsistent template hacks we can use to achieve the effect.

#include <iostream>
#include <type_traits>

template <typename T,
  typename=typename std::enable_if_t<std::is_integral_v<T>
    || std::is_floating_point_v<T>>
> auto add(T x, T y) -> T {
  return x + y;
}

int main(int argc, char**argv) {
  std::cout<<add(12, 15)<<' '
  <<add(1.2, 4.3)<<std::endl;
  // will not compile:
  // error: no matching function for call to ‘add(std::string, std::string)’
  // add(std::string("hello"), std::string(" world"));
  return 0;
}
