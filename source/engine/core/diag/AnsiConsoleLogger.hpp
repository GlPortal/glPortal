#ifndef ANSI_CONSOLE_LOGGER_HPP
#define ANSI_CONSOLE_LOGGER_HPP

#include <string>
#include <engine/core/diag/Logger.hpp>

namespace glPortal {

/** \class AnsiConsoleLogger
 * @brief Logger that outputs to an ANSI/vt-100 console
 */
class AnsiConsoleLogger : public Logger {
public:
  /**
   * @brief Enables colored output
   */
  bool enableColors;
  /**
   * @brief Enables colored backround on log tags
   */
  bool enableBackground;

  AnsiConsoleLogger();

  const char* getName() const;
  void log(const std::string &message, LogLevel lvl, const std::string &tag);
};
} /* namespace glPortal */

#endif /* ANSI_CONSOLE_LOGGER_HPP */
