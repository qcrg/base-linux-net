#include "cla.hxx"

#include <stdexcept>

namespace pnd
{
  cla_iterator::cla_iterator(int argc, const char **argv)
    : ptr{argv}
    , size{argc}
  {
  }

  cla_iterator cla_iterator::operator++()
  {
    ptr++;
    if (!--size)
      throw std::length_error("there are no more arguments");
    return *this;
  }

  cla_iterator cla_iterator::operator++(int)
  {
    return operator++();
  }

  const char *cla_iterator::operator*() const
  {
    return *ptr;
  }

  convertor::convertor(cla_iterator iter)
    : iter{iter}
  {
  }

  int convertor::port()
  {
    try {
      iter++;
      int port = std::stoi(*iter);
      if (port < 0 || port > 65535)
        throw std::invalid_argument(
            "port must be greater than or equal to 0 and less than 65535");
      return port;
    } catch (std::length_error &) {
      throw std::invalid_argument("port is not set");
    } catch (std::invalid_argument &) {
      throw std::invalid_argument("port is not integer");
    }
  }

  std::string convertor::name()
  {
    try {
      return *++iter;
    } catch (std::length_error &) {
      throw std::invalid_argument("name is not set");
    }
  }

  ch::seconds convertor::time()
  {
    try {
      return ch::seconds(std::stoi(*++iter));
    } catch (std::length_error &) {
      throw std::invalid_argument("time is not set");
    } catch (std::invalid_argument &) {
      throw std::invalid_argument("time is not integer");
    }
  }
} //namespace pnd
