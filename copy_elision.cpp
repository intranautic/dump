#include <iostream>
#include <string>

class copyme {
  friend std::ostream& operator<<(std::ostream& out, const copyme& c) {
    out << "Copied " << c.n << " times";
    return out;
  }
public:
  copyme(): n(0) {}
  copyme(int n)
    : n(n)
  {
  }
  copyme(const copyme&) { ++n; }
private:
  int n;
};

copyme foo() {
  auto x = copyme();
  int y = 3;
  return x;
}

copyme bar(int x) {
  copyme a, b;
  if (x)
    return a;
  return b;
}

int main(int argc, char** argv) {
  auto x = copyme();
  std::cout<<x<<std::endl;
  x = foo();
  std::cout<<x<<std::endl;
  return 0;
}
