#include "System.hpp"

#include <stdexcept>

namespace glPortal {

void System::log(std::string message) {
  logger.log(message);
}

} /* namespace glPortal */
