#include "System.hpp"

#include <util/logger/Logger.hpp>

namespace glPortal {

Logger System::logger;

void System::log(std::string message) {
  logger.log(message);
}

} /* namespace glPortal */
