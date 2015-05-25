#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <string>

namespace glPortal {

class Logger {
public:
  virtual void log(const std::string &message) {}
};

} /* namespace glPortal */

#endif /* LOGGER_HPP */
