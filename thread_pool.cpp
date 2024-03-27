#include <iostream>
#include <string>
#include <cstdint>

namespace thread_pool {
class pool_exception : public std::runtime_error {
public:
  explicit pool_exception(const std::string& msg)
    : std::runtime_error(msg)
  {
  }
  ~pool_exception() noexcept {}
private:
  friend std::ostream& operator<<(std::ostream& out, const pool_exception& e) {
    out << e.what();
    return out;
  }
};

class sync_heap {};
class policy {};

template <
  typename Task = std::function<void(void)>,
  typename Queue = sync_heap<Task>,
  typename
>
class thread_pool {};
} // ::thread_pool

int main(int argc, char** argv) {
  try {
    throw thread_pool::pool_exception("Could not fucking shit the thing");
  } catch (thread_pool::pool_exception& e) {
    std::cout << e << std::endl;
  }
  return 0;
}
