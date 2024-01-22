#include "network.hxx"

#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <stdexcept>
#include <system_error>
#include <cstring>
#include <iostream>

namespace pnd
{
  const uint32_t NETC::LOCALHOST = 0;

  int Network::_recv(void *buf, size_t len, int flags)
  {
    int n;
    do {
      n = recv(fd, buf, len, flags);
    } while (n == -1 && (errno == EAGAIN || errno == EINTR));
    return n;
  }

  int Network::_send(const void *buf, size_t len, int flags)
  {
    int n;
    do {
      n = send(fd, buf, len, flags);
    } while (n == -1 && errno == EINTR);
    return n;
  }

  Network::Network(int fd)
    : fd{fd}
  {
  }

  Network::Network(Network &&other)
    : fd{other.fd}
  {
    fd = other.fd;
    other.fd = -1;
  }

  Network &Network::operator=(Network &&other)
  {
    if (-1 != fd)
      close(fd);
    fd = other.fd;
    other.fd = -1;
    return *this;
  }

  Network::~Network()
  {
    if (-1 != fd)
      if (0 != close(fd))
        std::cerr << "Error on close socket with fd[" << fd
          << "]: "
          << strerror(errno)
          << std::endl;
  }

  ClientNetwork::ClientNetwork(int client_fd)
    : Network{client_fd}
  {
  }

  std::string ClientNetwork::recv()
  {
    std::string res(512, 0);
    int n = _recv(&res[0], res.size());
    if (-1 == n)
      throw std::system_error(errno, std::generic_category(),
          "Failed to receive message from fd [" + std::to_string(fd) + "]");
    res.resize(n);
    return res;
  }

  ServerNetwork::ServerNetwork(int port)
  {
    fd = socket(AF_INET, SOCK_STREAM, 0);
    if (-1 == fd)
      throw std::system_error(errno, std::generic_category(),
          "failed to create server socket");

    sockaddr_in addr;
    std::memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(port);
    if (0 != bind(fd, (sockaddr *)&addr, sizeof(addr)))
      throw std::system_error(errno, std::generic_category(),
          "failed to bind server socket");
    
    if (0 != listen(fd, 10))
      throw std::system_error(errno, std::generic_category(),
          "failed to listen server socket");
  }

  ClientNetwork ServerNetwork::accept()
  {
    int cfd;
    do {
      cfd = ::accept(fd, nullptr, 0);
    } while (-1 == cfd && (cfd == EINTR && cfd == EAGAIN));
    if (-1 == cfd)
      throw std::system_error(errno, std::generic_category(),
          "failed to accept connection on fd[" + std::to_string(fd) + "]");
    return ClientNetwork(cfd);
  }

  UserNetwork::UserNetwork(int fport, uint32_t ip)
  {
    fd = socket(AF_INET, SOCK_STREAM, 0);
    if (-1 == fd)
      throw std::system_error(errno, std::generic_category(),
          "failed to create client socket");
    
    sockaddr_in addr;
    std::memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(ip);
    addr.sin_port = htons(fport);

    int ret;
    do {
      ret = connect(fd, (sockaddr *)&addr, sizeof(addr));
    } while (-1 == ret && (errno == EAGAIN || errno == EINTR));
    if (-1 == ret)
      throw std::system_error(errno, std::generic_category(),
          "failed to connect");
  }

  void UserNetwork::send(const std::string &msg)
  {
    if (-1 == _send(msg.data(), msg.size()))
      throw std::system_error(errno, std::generic_category(),
          "failed to send message on fd[" + std::to_string(fd) + "]");
  }
} //namespace pnd