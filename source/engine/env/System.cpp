#include "System.hpp"

namespace glPortal {

ConsoleLogger System::logger;

void System::log(std::string message) {
  logger.log(message);
}

} /* namespace glPortal */
