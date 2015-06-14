#ifndef CONSOLE_LOGGER_HPP
#define CONSOLE_LOGGER_HPP

#include <engine/core/diag/Logger.hpp>
#include <string>

namespace glPortal {

class ConsoleLogger : public Logger {
public:
  const char* getName() const;
  void log(const std::string &message, LogLevel lvl);
};
} /* namespace glPortal */

#endif /* CONSOLE_LOGGER_HPP */
