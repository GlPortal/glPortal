#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <string>

namespace glPortal {

enum LogLevel {
  Verbose,
  Debug,
  Info,
  Warning,
  Error,
  Failure
};

class Logger {
public:
  virtual const char* getName() const;
  virtual void log(const std::string &message, LogLevel lvl, const std::string &tag) {}
};

} /* namespace glPortal */

#endif /* LOGGER_HPP */
