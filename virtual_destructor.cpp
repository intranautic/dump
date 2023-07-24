#include <iostream>

class cocksucker {
public:
  virtual ~cocksucker() {
    std::cout<<"cocksucker::~cocksucker called!"<<std::endl;
  }
};
class derived : public cocksucker {
public:
  ~derived() {
    std::cout<<"derived::~derived called!"<<std::endl;
  }
};

int main(int argc, char** argv) {
  cocksucker* x = new derived();
  delete x;
}
