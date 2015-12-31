#ifndef SYSTEM_HPP
#define SYSTEM_HPP

#include <string>
#include <memory>
#include <random>

#include <engine/core/diag/LogInput.hpp>

namespace glPortal {

class System {
private:
  static std::unique_ptr<Logger> logger;

public:
/*! \cond PRIVATE */
  struct _Log {
    LogInput operator()();
    LogInput operator()(LogLevel);
    LogInput operator()(LogLevel, const std::string &tag);
  };
/*! \endcond */
  static _Log Log;

  static std::random_device RandDev;
  static std::mt19937 Rand;

  static void Init();
};

} /* namespace glPortal */

#endif /* SYSTEM_HPP */
