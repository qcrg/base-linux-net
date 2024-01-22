#include "network.hxx"
#include "cla.hxx"
#include "time_point.hxx"

#include <iostream>
#include <thread>

using namespace pnd;

int main(int argc, const char **argv)
{
  convertor conv(cla_iterator(argc, argv));
  std::string name = conv.name();
  int port = conv.port();
  ch::seconds time = conv.time();
  UserNetwork net(port);
  for (;;) {
    auto begin = ch::system_clock::now();
    net.send(get_time() + " " + name);
    auto end = ch::system_clock::now();
    auto diff = end - begin;
    if (diff > time)
      continue;
    std::this_thread::sleep_for(time - diff);
  }
  return 0;
}