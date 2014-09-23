#ifndef SYSTEM_HPP
#define SYSTEM_HPP

#include <string>
#include <util/logger/ConsoleLogger.hpp>

namespace glPortal {

class System {
private:
  static Logger logger;
public:
  static void log(std::string message);
};

} /* namespace glPortal */

#endif /* SYSTEM_HPP */
