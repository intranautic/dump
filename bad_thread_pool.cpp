// throwing this away, bad to inherit std containers due to lack of
// virtual destructors and destroys heirarchy. bad on you sobyte man
// https://www.sobyte.net/post/2022-05/design-a-thread-pool/
#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <future>
#include <queue>
#include <shared_mutex>

namespace thread_pool {
class pool_policy {
public:
};

/*
 * std::shared_mutex allows shared acquisition of reader locks and exclusive
 * access for write. allows for more throughput by reducing redundency on
 * sync operations.
 */
template <typename T>
class work_queue : protected std::queue<T> {
  using queue_t = std::queue<T>;
  using rlock_t = std::shared_lock<std::shared_mutex>;
  using wlock_t = std::unique_lock<std::shared_mutex>;
public:
  work_queue()=default;
  ~work_queue() {}
  // TODO: implement proper sync copy/move
  work_queue(const work_queue& _wq) {
    // acquire copy-from object read lock
    rlock_t rlk(_wq._lock);
    // acquire this object write lock
    wlock_t wlk(this->_lock);
    queue_t::queue_t(_wq);
  }
  work_queue(work_queue&& _wq)=delete;
  work_queue& operator=(const work_queue& _wq)=delete;
  work_queue& operator=(work_queue&& _wq)=delete;

  auto front() const -> const T& { return queue_t::front(); }
  auto back() const -> const T& { return queue_t::back(); }
  bool empty() const {
    rlock_t lock(this->_lock);
    return queue_t::empty();
  }
  size_t size() const {
    rlock_t lock(this->_lock);
    return queue_t::size();
  }
  void push(const T& value) {
    wlock_t lock(this->_lock);
    queue_t::push(value);
    return;
  }
  void push(T&& value) {
    wlock_t lock(this->_lock);
    queue_t::push(std::forward<T>(value));
    return;
  }
  template <typename... Args>
  void emplace(Args... args) {
    wlock_t lock(this->_lock);
    queue_t::emplace(std::forward<Args>(args)...);
    return;
  }
  bool pop(T& hold) {
    wlock_t lock(this->_lock);
    if (queue_t::empty())
      return false;
    hold = std::move(queue_t::front());
    queue_t::pop();
    return true;
  }
  void swap(work_queue& other) noexcept {}
private:
  std::shared_mutex _lock;
};

class pool_thread {};
class pool_worker {};


template <
  typename Task=pool_worker,
  typename Queue = work_queue<Task>
> class thread_pool {
  using wqueue_t = Queue;
  using task_t = std::function<int(void *)>;
public:
  bool is_running() const;
  size_t size() const { return _tasks.size(); }

  template <class F, class... Args>
  auto async(F&& f, Args&&... args) const ->
    std::future<decltype(f(args...))>
  {
  }
private:
  std::vector<std::thread> _pool;
  wqueue_t                 _tasks;
};
} // ::thread_pool

int main(int argc, char** argv) {
  thread_pool::work_queue<int> x;
  auto y = x;
  return 0;
}
