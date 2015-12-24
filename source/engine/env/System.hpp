#ifndef SYSTEM_HPP
#define SYSTEM_HPP

#include <string>
#include <memory>
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

  static void Init();
};

} /* namespace glPortal */

#endif /* SYSTEM_HPP */
