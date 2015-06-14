#include "ConsoleLogger.hpp"

#include <iostream>

namespace glPortal {

const char* ConsoleLogger::getName() const {
  return "Console logger";
}

void ConsoleLogger::log(const std::string &message, LogLevel lvl) {
  std::cout << message << std::endl;
}

} /* namespace glPortal */
