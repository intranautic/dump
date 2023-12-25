#include <bits/stdc++.h>

class Resource {
  friend std::ostream& operator<<(std::ostream& out, Resource& rsrc) {
    out << "Resource {\n  buffer: " << rsrc._buffer
      << "\n  length: " << rsrc._length << "\n}\n";
    return out;
  }
public:
  Resource(char fill='A', int length=50)
    : _length(length)
  {
    if (_length) {
      _buffer = (char *)calloc(1, length + 1);
      memset(_buffer, fill, length);
    }
  }
  ~Resource() {
    if (_buffer)
      free(_buffer);
  }

  // copy constructor
  Resource(const Resource& _rhs) {
    if (_rhs._length && _rhs._buffer) {
      if (this->_buffer)
        free(this->_buffer);
      this->_length = _rhs._length;
      this->_buffer = (char *)calloc(1, this->_length + 1);
      memcpy(this->_buffer, _rhs._buffer, this->_length);
    }
  }
  // move constructor
  Resource(Resource&& _rhs) noexcept
    : _buffer(_rhs._buffer)
    , _length(_rhs._length)
  {
    _rhs._buffer = nullptr;
    _rhs._length = 0;
  }
  // copy assign constructor, perform deep copy
  Resource& operator=(const Resource& _rhs) {
    if (_rhs._length && _rhs._buffer) {
      if (this->_buffer)
        free(this->_buffer);
      this->_length = _rhs._length;
      this->_buffer = (char *)malloc(this->_length + 1);
      memcpy(this->_buffer, _rhs._buffer, this->_length);
    }
    return *this;
  }
  Resource& operator=(Resource&& _rhs) noexcept {
    _buffer = _rhs._buffer;
    _length = _rhs._length;
    _rhs._buffer = nullptr;
    _rhs._length = 0;
    return *this;
  }
private:
  char* _buffer;
  int _length;
};

int main(int argc, char** argv) {
  Resource x, y;
  std::cout << x << y << std::endl;

  return 0;
}
