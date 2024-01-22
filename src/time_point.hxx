#pragma once

#include <string>
#include <ctime>
#include <chrono>
#include <sstream>
#include <iomanip>

namespace pnd
{
  std::string get_time()
  {
    using namespace std::chrono;
    time_t point = std::time(nullptr);
    int mls = duration_cast<milliseconds>(
        system_clock::now().time_since_epoch()).count() % 1000;
    
    std::stringstream ss;
    ss << "["
      << std::put_time(std::gmtime(&point), "%Y-%m-%d %H:%M:%S.")
      << std::setw(3) << std::setfill('0') << std::to_string(mls)
      << "]";
    return ss.str();
  }
} //namespace pnd
