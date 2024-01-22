#include "network.hxx"
#include "cla.hxx"

#include <iostream>
#include <thread>
#include <fstream>

using namespace pnd;

void thr(std::ofstream &out, ClientNetwork net)
{
  for (;;) {
    try {
      std::string msg = net.recv();
      out << msg << std::endl;
      std::clog << msg << std::endl;
    } catch (const std::exception &ex) {
      std::cerr << "client: "
        << ex.what()
        << std::endl;
    }
  }
}

int main(int argc, const char **argv)
{
  convertor conv(cla_iterator(argc, argv));
  int port = conv.port();
  std::ofstream file("log.txt", std::ios::out | std::ios::app);
  if (!file.is_open())
    throw std::runtime_error("failed to open log.txt");

  ServerNetwork server(port);
  for (;;) {
    try {
      ClientNetwork client = server.accept();
      std::clog << *reinterpret_cast<int *>(&client) << std::endl;
      std::thread(thr, std::ref(file), std::move(client)).detach();
    } catch (const std::exception &ex) {
      std::cerr << "server: "
        << ex.what()
        << std::endl;
    }
  }
  return 0;
}
