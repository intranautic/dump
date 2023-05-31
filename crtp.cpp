// crtp (curiously recurring template pattern) is a pattern which allows us to achieve static
// polymorphism at compile time. 
#include <iostream>

template <typename T>
class base {
public:
  void interface(void) {
    static_cast<T*>(this)->action();
    return;
  }
  void action(void) const {
    std::cout<<"base"<<std::endl;
  }
};

class derived1 : public base<derived1> {
public:
  void action(void) const {
    std::cout<<"derived1"<<std::endl;
  }
};

class derived2 : public base<derived2> {
public:
  void action(void) const {
    std::cout<<"derived1"<<std::endl;
  }
};

// obviously better means of implementing this
template <typename T>
void execute(T& base) {
  base.interface();
  return;
}

int main(int argc, char**argv) {
  derived1 d1;
  derived2 d2;
  execute(d1);
  execute(d2);
  return 0;
}
