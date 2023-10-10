// custom template metaprogramming library from scratch

struct true_type {
  static constexpr bool value = true;
};
struct false_type {
  static constexpr bool value = false;
};
template <bool B, typename Then, typename Else>
struct conditional;
template <typename Then, typename Else>
struct conditional<true, Then, Else> {
  using type = Then;
};
template <typename Then, typename Else>
struct conditional<false, Then, Else> {
  using type = Else;
};
template <typename A, typename B>
struct is_same : false_type {};
template <typename A>
struct is_same<A, A> : true_type {};

template <bool B, typename T>
struct enable_if {};
template <typename T>
struct enable_if<true, T> {
  using type = T;
};

// type lists
template <typename...>
struct type_list {};

template <typename List>
struct typelist_empty : false_type {};
template <>
struct typelist_empty<type_list<>> : true_type {};

template <typename List>
struct typelist_front;
template <typename T, typename... Args>
struct typelist_front<type_list<T, Args...>> {
  using type = T;
};

template <typename List>
struct typelist_pop_front;
template <typename T, typename... Args>
struct typelist_pop_front<type_list<T, Args...>> {
  using type = type_list<Args...>;
};

template <typename S, typename List>
struct typelist_contains
: conditional<
    typelist_empty<List>::value,
    false_type,
    typename conditional<
      is_same<S, typelist_front<List>::type>::value,
      true_type,
      typelist_contains<S, typelist_pop_front<List>>
    >::type
  >::type
{
};

int main(int argc, char**argv) {

  return 0;
}

