#include "ConsoleLogger.hpp"

namespace glPortal {

void ConsoleLogger::log(std::string message) {
  std::cout << message << std::endl;
}

} /* namespace glPortal */
