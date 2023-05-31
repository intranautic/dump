// type erasure is an idiom which aims to provide a type generic container
// which interfaces a variety or class of concrete types. this idiom is employed
// within std::any and std::function.
// reference: https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Type_Erasure
#include <iostream>

class human {
public:
  // must introduce
  human(void)=delete;
  human(const std::string& name, int age)
    : m_name(name.c_str())
    , m_age(age)
  {
  }
  // humans cannot be copied
  human(const human&)=delete;
  human& operator=(const human&)=delete;
  // but they can be moved
  human(human&& _human)
    : m_name(std::move(_human.m_name))
    , m_age(_human.m_age)
  {
    _human.m_name = 0;
    _human.m_age = 0;
  }
  human& operator=(human&& _human) {
    if (this != &_human) {
      m_name = std::move(_human.m_name);
      m_age = _human.m_age;
      _human.m_name = 0;
      _human.m_age = 0;
    }
    return *this;
  }
  virtual ~human(void)=default;
  virtual void introduce(void) {
    std::cout << "My name is " << m_name
      << " and I am " << m_age << " years old.\n";
    return;
  }
private:
  const char* m_name;
  int m_age;
};

class male : public human {
  using _base = human;
public:
  male(const std::string& name, int age)
    : _base(name, age)
  {
  }
  void introduce(void) override {
    _base::introduce();
    std::cout << "i am also a male." << std::endl;
    return;
  }
};

class female : public human {
  using _base = human;
public:
  female(const std::string& name, int age)
    : _base(name, age)
  {
  }
  void introduce(void) override {
    _base::introduce();
    std::cout << "i am also a female." << std::endl;
    return;
  }
};

// inheritance is a good solution to both static and runtime polymorphism
// in the form of the rtti and crtp patterns. however, there are scenarios
// in which you want to have a generic interface which interacts with multiple
// types which do not inherit from the same base; or possibly even literals
// which do not inherit from any base at all.
// this is an example of a common interface which only accepts types which
// inherit from the base human type. 

// void introduce_human(human& _human) {
//   _human.introduce();
//   return;
// }

// type erasure is simply the removal of the static type which is assigned to
// our interface. this concept is also used in dynamically typed interpreted
// languages called "ducktyping". "if it walks like a duck and quacks like a
// duck, then it must be a duck". this simply means that, regardless of the
// type, as long as the required functionality/members are provided, the
// interface will conform to them all. here is an example of this.
// a benefit that this grants us as apposed to inheritance based polymorphism
// is the fact that we retain the type of the argument passed, whilst it is
// lost via inheritance.
template <typename T>
void introduce_human(T& _human) {
  // regardless if T has been inherited from 
  _human.introduce();
  return;
}

// does not inherit from human
class imposter {
public:
  void introduce(void) {
    std::cout << "My name mark zuckyberk and im a couple centuries old.\n"
      << "trust me, i am human.." <<std::endl;
    return;
  }
};

namespace fucker {

};

int main(int argc, char**argv) {
  // silly example 
  male todd("todd", 43);
  introduce_human(todd);
  female sasha("sasha", 21);
  introduce_human(sasha);
  imposter mark_zuckerberg;
  introduce_human(mark_zuckerberg);
  return 0;
}
