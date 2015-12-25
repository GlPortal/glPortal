#ifndef LOGINPUT_HPP
#define LOGINPUT_HPP

#include <string>
#include "Logger.hpp"

namespace glPortal {

class Vector2f;
class Vector3f;
class Vector4f;

/** \class LogInput
 * @brief `std::cout`-like object that sends the data it aggregated to a Logger when it dies
 */
class LogInput {
protected:
  Logger &sink;
  LogLevel lvl;
  std::string buf, tag;

public:
  LogInput(Logger &sink, LogLevel lvl, const std::string &tag = "");
  ~LogInput();

  LogInput(const LogInput&) = delete;
  LogInput& operator=(const LogInput&) = delete;

  LogInput(LogInput&&);
  LogInput& operator=(LogInput&&) = delete;

  LogInput& operator<<(const char*);
  LogInput& operator<<(const std::string&);

  LogInput& operator<<(bool);
  LogInput& operator<<(char);
  LogInput& operator<<(uint8_t);
  LogInput& operator<<(int8_t);
  LogInput& operator<<(uint16_t);
  LogInput& operator<<(int16_t);
  LogInput& operator<<(uint32_t);
  LogInput& operator<<(int32_t);
  LogInput& operator<<(uint64_t);
  LogInput& operator<<(int64_t);
  LogInput& operator<<(float);
  LogInput& operator<<(double);
  LogInput& operator<<(const void*);

  LogInput& operator<<(const Vector2f&);
  LogInput& operator<<(const Vector3f&);
  LogInput& operator<<(const Vector4f&);
};

} /* namespace glPortal */

#endif /* LOGINPUT_HPP */
