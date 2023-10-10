#include <iostream>
#include <type_traits>

// since c++20
template <typename T>
struct _remove_cvref {
  typedef std::remove_cv_t<std::remove_reference_t<T>> type;
};
template <typename T> using _remove_cvref_t = typename _remove_cvref<T>::type;

class person {
public:
  // solves the problem of code repition with overloading the constructor with
  // a const copy constructor and a non-const move constructor for std::string
  template <typename T,
    typename = typename std::enable_if_t<
      std::is_convertible_v<_remove_cvref_t<T>, std::string>,
    int>
  > person(T&& name)
    : m_name(std::forward<T>(name))
  {
  }

  std::string get_name() const { return m_name; }
private:
  std::string m_name;
};

int main(int argc, char** argv) {
  auto john = person("John");
  std::cout << john.get_name() << '\n';
  return 0;
}

