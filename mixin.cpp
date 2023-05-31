#include <iostream>
#include <string>

template <typename T>
class noncopyable : public T {
public:
  noncopyable(const noncopyable&)=delete;
  T& operator=(const T&)=delete;
protected:
  noncopyable(void)=default;
  ~noncopyable(void)=default;
};

class base {
public:
  virtual const std::string hello(void) {
    return "base";
  }
};

// noncopyable inherits from base
// derived inherits from noncopyable
class derived : public noncopyable<base> {
public:
  const std::string hello(void) override {
    return "derived";
  }
};

int main(int argc, char**argv) {
  derived d;
  std::cout<< d.hello() << std::endl;
  return 0;
}
