#include "ConsoleLogger.hpp"

#include <iostream>

namespace glPortal {

void ConsoleLogger::log(const std::string &message) {
  std::cout << message << std::endl;
}

} /* namespace glPortal */
