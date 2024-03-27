// simple implementation of kademlia dht
// https://cs.nyu.edu/~anirudh/CSCI-GA.2620-001/papers/kademlia.pdf
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <stdexcept>
#include <optional>


#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

namespace kademlia {
class kademlia_error : public std::runtime_error {
  using super_t = std::runtime_error;
public:
  using super_t::super_t;
  const char* what() const noexcept override {
    return super_t::what();
  }
};

template <typename Addr>
class socket {
public:
  using handle_t = int;
  static const handle_t invalid_socket = -1;

  explicit socket(handle_t handle) noexcept
    : _handle(handle)
  {
  }
  socket() : _handle(invalid_socket) {}
  socket(socket&& _sock) noexcept
    : socket(_sock._handle)
  {
    _sock._handle = invalid_socket;
  }
  socket& operator=(socket&& _sock) noexcept {
    this->_handle = _sock.handle;
    _sock.handle = invalid_socket;
  }
  virtual ~socket() {
    if (this->is_open())
      this->close();
  }
  socket(const socket&)=delete;
  socket& operator=(const socket&)=delete;

  int get_handle() const { return _handle; }
  bool is_open() const { return _handle != invalid_socket; }
  bool operator!() const { return !this->is_open(); }
  explicit operator bool() const { return this->is_open(); }
  int close() noexcept {
    int tmp = invalid_socket;
    std::swap(this->_handle, tmp);
    return ::close(tmp);
  }

  template <typename T>
  int set_option(int level, int option, T* value) noexcept {}
  template <typename T>
  std::optional<T> get_option(int level, int option, T* value) const noexcept {
    socklen_t len = sizeof(T);
    return getsockopt(this->handle, level, option, value, &len);
  }
private:
  handle_t _handle;
};

template <typename Addr>
class channel : public socket<Addr> {};

template <typename Sock, typename Addr>
class connector {};
template <typename Sock, typename Addr>
class acceptor {};

class bucket {};
class node {};
class dht {};

} // ::kademlia

int main(int argc, char** argv) {

  return 0;
}
