#include <stdio.h>
#include <stdlib.h>

void* operator new(size_t size) noexcept {
  return malloc(size);
}

void operator delete(void* p) noexcept {
  free(p);
  return;
}

void* operator new[](size_t size) noexcept {
  return operator new(size);
}

void operator delete[](void* p) noexcept {
  operator delete(p);
  return;
}

class human {
  const char* _name;
  int _age;
public:
  human(const char* name, int age)
    : _name(name)
    , _age(age)
  {
  }
  virtual ~human()=default;
  virtual int speak() const {
    return printf("My name is %s and I am %d years old\n", _name, _age);
  }
};

class icecream_man : public human {
  using _base = ::human;
public:
  icecream_man(const char* name, int age)
    : _base(name, age)
  {
  }
  ~icecream_man()=default;
  int speak() const override {
    return _base::speak() + puts("I also love ice cream");
  }
};

int main(int argc, char** argv) {
  auto x = new icecream_man("jeremy", 21);
  x->speak();
  delete x;
  return 0;
}

