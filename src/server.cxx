#include "network.hxx"
#include "cla.hxx"

#include <iostream>
#include <thread>
#include <fstream>
#include <mutex>

using namespace pnd;

std::mutex filemut;

void thr(std::ofstream &out, ClientNetwork net)
{
  try {
    for (;;) {
      std::string msg = net.recv();
      if (0 == msg.size())
        break;
      std::lock_guard<std::mutex> guard(filemut);
      out << msg << std::endl;
      std::clog << msg << std::endl;
    }
  } catch (const std::exception &ex) {
    std::cerr << "client: "
      << ex.what()
      << std::endl;
  }
}

int main(int argc, const char **argv)
{
  // convertor conv(cla_iterator(argc, argv));
  // int port = conv.port();
  int port = 3000;
  std::ofstream file("log.txt", std::ios::out | std::ios::app);
  if (!file.is_open())
    throw std::runtime_error("failed to open log.txt");

  ServerNetwork server(port);
  for (;;) {
    try {
      ClientNetwork client = server.accept();
      std::thread(thr, std::ref(file), std::move(client)).detach();
    } catch (const std::exception &ex) {
      std::cerr << "server: "
        << ex.what()
        << std::endl;
    }
  }
  return 0;
}
