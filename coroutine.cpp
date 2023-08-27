// testing out c++20 coroutines
#include <cstdio>
#include <iostream>
#include <coroutine>
#include <thread>

struct promise;
struct coroutine : std::coroutine_handle<promise> {
  using promise_type = ::promise;
};

struct promise {
  coroutine get_return_object() {
    return {coroutine::from_promise(*this)};
  }
  // never suspend, when co_await invoked for the first time on this
  // promise, it will not suspend the coroutine.
  std::suspend_never initial_suspend() noexcept {
    puts("initial_suspend");
    return {};
  }
  // always suspend coroutine when co_await invoked
  std::suspend_always final_suspend() noexcept {
    puts("final_suspend");
    return {};
  }
  void return_void() {}
  void unhandled_exception() {}
};


struct task {
  int n;
  task() {}
  task(int n)
    : n(n)
  {
  }
  void inc() { n++; }
  void print() const {
    std::cout << n << std::endl;
  }
};

// custom awaitable, passed as co_await <expr>
auto await_task(task* t) {
  struct awaitable {
    task* t;
    bool await_ready() { return false; }
    void await_suspend(std::coroutine_handle<>) {
      std::jthread([i=t] {
        i->inc();
      });
    }
    void await_resume() {}
  };
  return awaitable{t};
}

auto coroutine_factory(task* t) {
  return [i=t]() -> coroutine {
    puts("resume #1");
    co_await await_task(i);
    puts("resume #2");
    co_await await_task(i);
    puts("resume #3");
    co_await await_task(i);
    puts("resume #4");
    i->print();
    co_return;
  }();
}


int main(void) {
  task t(0);
  auto task_coroutine = coroutine_factory(&t);

  task_coroutine.resume();
  task_coroutine.resume();
  task_coroutine.resume();
  return 0;
}
