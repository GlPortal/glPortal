#ifndef SYSTEM_HPP
#define SYSTEM_HPP

#include <string>
#include <engine/core/diag/ConsoleLogger.hpp>

namespace glPortal {

class System {
private:
  static ConsoleLogger logger;
public:
  static void log(std::string message);
};

} /* namespace glPortal */

#endif /* SYSTEM_HPP */
