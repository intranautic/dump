/*
  when you define a container via templates, you change the type of the container
  itself. this is due to c++'s strong type system which is very strict. this means
  that if we define something like a vector with a custom allocator, it is no longer
  the same type as the default stl's std::vector. you can't pass it to a function or
  method which requires the default allocator.

  a polymorphic allocator is a type with a member that defines the behavior through
  dynamic dispatch (virtual tables), which allows you to have containers with use
  a custom allocator to share a common type.
*/

#include <iostream>
#include <memory>
#include <memory_resource>
#include <vector>
#include <limits>

// basic custom allocator uses default system implementation
template <typename T>
class custom_allocator {
public:
  using value_type = T;

  custom_allocator()=default;

  template <typename U>
  constexpr custom_allocator(const custom_allocator<U>&) noexcept
  {
  }

  [[nodiscard]] value_type* allocate(std::size_t n) {
    // if allocate invalid size, throw bad array length exception
    if (n > std::numeric_limits<std::size_t>::max() / sizeof(value_type))
      throw std::bad_array_new_length();
    // allocate chunk from default system allocator
    auto ptr = (T *)std::malloc(n * sizeof(value_type));
    // if success return pointer to chunk
    if (ptr)
      return ptr;
    // else throw bad alloc exception
    throw std::bad_alloc();
  }

  void deallocate(value_type* ptr, std::size_t n) noexcept {
    // free chunk
    std::free(ptr);
    return;
  }
};

// minimal implementation of a memory resource
// uses std::pmr::new_delete_resource() to alloc/dealloc memory
class custom_resource : public std::pmr::memory_resource {
private:
  void* do_allocate(std::size_t n, std::size_t align) override {
    return std::pmr::new_delete_resource()->allocate(n, align);
  }

  void do_deallocate(void* ptr, std::size_t n, std::size_t align) override {
    std::pmr::new_delete_resource()->deallocate(ptr, n, align);
    return;
  }

  bool do_is_equal(const std::pmr::memory_resource& other) const noexcept override {
    return std::pmr::new_delete_resource()->is_equal(other);
  }
};

int main(int argc, char** argv) {
  std::pmr::vector<int> v = {1, 2, 3, 4, 5};
  auto cv = std::vector<int, custom_allocator<int>>();

  auto custom_res = custom_resource();
  auto pmr_alloc = std::pmr::polymorphic_allocator<int>(&custom_res);
  std::pmr::vector<int> pv {pmr_alloc};
  pv = v;
  v = pv;
  return 0;
}
