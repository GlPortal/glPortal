#include "System.hpp"

namespace glPortal {

std::unique_ptr<Logger> System::logger;
System::_Log System::Log;

void System::Init() {
  logger.reset(new ConsoleLogger);
}

LogInput System::_Log::operator()() {
  return LogInput(*System::logger, LogLevel::Debug);
}

LogInput System::_Log::operator()(LogLevel lvl) {
  return LogInput(*System::logger, lvl);
}

LogInput System::_Log::operator()(LogLevel lvl, const std::string &tag) {
  return LogInput(*System::logger, lvl, tag);
}

} /* namespace glPortal */
