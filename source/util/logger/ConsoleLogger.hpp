#ifndef CONSOLE_LOGGER_HPP
#define CONSOLE_LOGGER_HPP

#include <string>
#include <iostream>
#include "Logger.hpp"

namespace glPortal {

class ConsoleLogger : public Logger {
public:
  void log(std::string message);
};
} /* namespace glPortal */

#endif /* CONSOLE_LOGGER_HPP */
