// c++ sfinae detection idiom
// this idiom utilizes sfinae to detect the presence of an attribute, method, or type
// within a class.

// c++20 concepts provides a much simpler means of creating semantic constraints on template
// parameters with the use of requires expressions. concepts are highly recommended for this as
// they provide a much more readable & less hacky means of achieving the same effect.
#include <iostream>
#include <type_traits>

// ostream mixin
template <typename T>
class make_ostreamable : private T {
public:
  friend std::ostream& operator<<(std::ostream& out, const T& v) {
    out << v;
    return out;
  }
};

// detect if type has been overloaded for stream
template <typename S, typename T, typename=void>
struct is_to_stream_writeable : std::false_type {};
template <typename S, typename T>
struct is_to_stream_writeable<S, T,
  typename std::void_t<decltype(std::declval<S>() << std::declval<T>())>
> : std::true_type {};

// detect if ostream operator has been overloaded
template <typename T, typename=void>
struct has_ostream : std::false_type {};
template <typename T>
struct has_ostream<T,
  typename std::enable_if_t<std::is_member_pointer<decltype(&T::operator<<)>::value>
> : std::true_type {};

class human {};

class quiet_human : public human {

};

class loud_human : public make_ostreamable<human> {
public:

};

int main(int argc, char**argv) {
  return 0;
}
