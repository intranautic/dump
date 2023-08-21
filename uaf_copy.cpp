// playing around with c++ copy uaf
// c++ is hell... this is hell..
#include <iostream>
#include <cstring>
#include <string>

class Human {
public:
  Human()=default;
  Human(const std::string& name, int age)
    : m_name(new char[name.length() + 1])
    , m_age(age)
  {
    strcpy(m_name, name.c_str());
    m_name[name.length()] = 0;
  }

  virtual ~Human() {
    if (m_name)
      delete[] m_name;
  }
  // if we dont explicitly define a deep copy for our dynamically allocated resource
  // which doesnt have a copy constructor either (which means it wont be invoked by default
  // copy; then the copy will default to shallow copy.
/*
  Human(const Human& _human)
    : m_name(new char[strlen(_human.m_name) + 1])
    , m_age(_human.m_age)
  {
    strcpy(m_name, _human.m_name);
    m_name[strlen(_human.m_name)] = 0;
    puts("COPY CONSTRUCTOR INVOKED");
  }
*/
  virtual void speak() const {
    std::cout
      << "My name is " << m_name << " and I am "
      << m_age << " years old, ";
  }
protected:
  char* m_name;
  int m_age;
};

class A : public Human {
  using _super = ::Human;
public:
  using _super::_super;
  virtual void speak() const override {
    _super::speak(); 
    std::cout << "I also really like ice cream.\n";
  }
};

// type erasure
template <typename T>
void speak(T speaking_object) {
  speaking_object.speak(); //heh
  return;
}

int main(int argc, char** argv) {
  auto x = new A("Todd", 21);

  // when not passing object by value, not by either reference or pointer, the object
  // is copied and the copy constructor is invoked.

  // delete  
  speak(*x);

  // shallow copy ref, destructor double free
  A y;
  y = *x;
  // this can lead to uaf bugs if a copy constructor is not explicitly declared fr a
  // resource which should be dynamically copied. as type char* doesnt have a copy
  // constructor either it will default to shallow copying the pointer when passing by
  // value -> invoking (default) copy constructor

  // destructor
  return 0;
}
