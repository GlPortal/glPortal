#ifndef STDOUT_LOGGER_HPP
#define STDOUT_LOGGER_HPP

#include <string>
#include <engine/core/diag/Logger.hpp>

namespace glPortal {

/** \class StdoutLogger
 * @brief Logger that outputs to an ANSI/vt-100 console
 */
class StdoutLogger : public Logger {
public:
  const char* getName() const;
  void log(const std::string &message, LogLevel lvl, const std::string &tag);
};

} /* namespace glPortal */

#endif /* STDOUT_LOGGER_HPP */
