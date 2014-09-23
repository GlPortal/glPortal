#ifndef CONSOLE_LOGGER_HPP
#define CONSOLE_LOGGER_HPP

#include <util/logger/Logger.hpp>
#include <string>

namespace glPortal {

class ConsoleLogger : public Logger {
public:
  ConsoleLogger();
  void log(std::string message);
};
} /* namespace glPortal */

#endif /* CONSOLE_LOGGER_HPP */
