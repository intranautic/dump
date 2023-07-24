#ifndef A_HPP_
#define A_HPP_
#include <cstdio>
struct A {
  int a;
  virtual void f() const { puts("A"); }
};
struct B : A {
  int b;
  void f() const override { puts("B"); }
};
struct C : A {
  int c;
  void f() const override { puts("C"); }
};
#endif // A_HPP_
