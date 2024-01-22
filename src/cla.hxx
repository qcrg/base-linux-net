#pragma once

#include <string>
#include <chrono>

namespace pnd
{
  namespace ch = std::chrono;

  class cla_iterator
  {
  public:
    cla_iterator(int argc, const char **argv);
    cla_iterator operator++();
    cla_iterator operator++(int);
    const char *operator*() const;
  private:
    const char **ptr;
    int size;
  };

  class convertor
  {
  public:
    convertor(cla_iterator iter);
    int port();
    std::string name();
    ch::seconds time();
  private:
    cla_iterator iter;
  };

  struct check_port
  {
    check_port(cla_iterator iter);
  };

  struct check_name
  {
    check_name(cla_iterator iter);
  };

  struct check_time
  {
    check_time(cla_iterator iter);
  };
} //namespace pnd