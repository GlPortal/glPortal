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

/** \class Logger
 * @brief Base class to create log sinks
 */
class Logger {
public:
  /**
   * @brief Returns the name of the Logger instance
   */
  virtual const char* getName() const;

  /**
   * @brief Log a message
   *
   * @param message Message to log
   * @param lvl     Log level
   * @param tag     Log tag, identifying the log category / class emitting the message
   */
  virtual void log(const std::string &message, LogLevel lvl, const std::string &tag) {}
};

} /* namespace glPortal */

#endif /* LOGGER_HPP */
