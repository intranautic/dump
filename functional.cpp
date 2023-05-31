// playing around with functional header and useless fp in c++
#include <iostream>
#include <functional>
#include <type_traits>

// useless but very cool for fp geeks
// also, is there no type trait for functor types within c++? std::is_invocable doesnt seem
// to work on the lambda function passed to it. and std::is_function only applies to static
// functions, so how am i supposed to deduce functors?
template <typename T>
struct y {
  T f;
  template <typename... Args>
  auto operator() (Args&&... args) const {
    return f(*this, std::forward<Args>(args)...);
  }
};
template <typename T>
y<T> fix(T&& fun) { return y<T>{std::forward<T>(fun)}; }

int main(int argc, char**argv) {
  // fibbonaci with anonymous recursion via fixed point combinator
  std::cout <<
    fix (
      [] (auto self, int n) -> int {
        return (n <= 1)
          ? n
          : (self(n - 1) + self(n - 2));
      }
    )(7)
  << std::endl;

  // std::bind usage
  // fairly useful, allows for partial application of "abstraction"s within c++
  // useless example, only serves to demonstrate basic usage of std::bind
  // currying using partial application with std::bind
  auto add = [] (int x, int y) { return x + y; };
  int result = std::bind(add, std::placeholders::_1, 5)(3);
  std::cout<<result<<std::endl;
  // signature is now: int add_prime(int x) { return x + 5 };
  auto add_prime = std::bind(add, std::placeholders::_1, 5);
  std::cout<<add_prime(5)<<std::endl;
  return 0;
}
