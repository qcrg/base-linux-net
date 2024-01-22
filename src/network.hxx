#pragma once

#include <cstdint>
#include <string>

namespace pnd
{
  struct NETC
  {
    static const uint32_t LOCALHOST;
  };

  class Network
  {
  public:
    Network() = default;
    Network(int fd);
    Network(Network &&other);
    ~Network();
    Network &operator=(Network &&other);
    Network &operator=(const Network &) = delete;
  protected:
    int _recv(void *buffer, size_t len, int flags = 0);
    int _send(const void *buffer, size_t len, int flags = 0);
    int fd{-1};
  };

  class ClientNetwork : public Network
  {
  public:
    ClientNetwork(int client_fd);
    std::string recv();
  };

  class ServerNetwork : public Network
  {
  public:
    ServerNetwork(int port);
    ClientNetwork accept();
  };

  class UserNetwork : public Network
  {
  public:
    UserNetwork(int foreign_port, uint32_t ip = NETC::LOCALHOST);
    void send(const std::string &msg);
  };
} //namespace pnd