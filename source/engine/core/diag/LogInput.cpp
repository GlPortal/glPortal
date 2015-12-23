#include "LogInput.hpp"
#include <iomanip>
#include <sstream>
#include <string>

#include <engine/core/math/Vector2f.hpp>
#include <engine/core/math/Vector3f.hpp>
#include <engine/core/math/Vector4f.hpp>

namespace glPortal {

LogInput::LogInput(Logger &sink, LogLevel lvl, const std::string &tag) :
  sink(sink), lvl(lvl), tag(tag) {
}

LogInput::~LogInput() {
  if (not buf.empty()) {
    sink.log(buf, lvl, tag);
  }
}

LogInput::LogInput(LogInput &&o) :
  sink(o.sink), lvl(o.lvl), buf(o.buf) {
  o.buf.clear();
}

LogInput& LogInput::operator<<(const char *s) {
  buf.append(s);
  return *this;
}

LogInput& LogInput::operator<<(const std::string &s) {
  buf.append(s);
  return *this;
}


LogInput& LogInput::operator<<(bool b) {
  buf.append(b ? "true" : "false");
  return *this;
}

LogInput& LogInput::operator<<(char c) {
  buf.push_back(c);
  return *this;
}

LogInput& LogInput::operator<<(uint8_t i) {
  buf.append(std::to_string(i));
  return *this;
}

LogInput& LogInput::operator<<(int8_t i) {
  buf.append(std::to_string(i));
  return *this;
}

LogInput& LogInput::operator<<(uint16_t i) {
  buf.append(std::to_string(i));
  return *this;
}

LogInput& LogInput::operator<<(int16_t i) {
  buf.append(std::to_string(i));
  return *this;
}

LogInput& LogInput::operator<<(uint32_t i) {
  buf.append(std::to_string(i));
  return *this;
}

LogInput& LogInput::operator<<(int32_t i) {
  buf.append(std::to_string(i));
  return *this;
}

LogInput& LogInput::operator<<(uint64_t i) {
  buf.append(std::to_string(i));
  return *this;
}

LogInput& LogInput::operator<<(int64_t i) {
  buf.append(std::to_string(i));
  return *this;
}

LogInput& LogInput::operator<<(float f) {
  buf.append(std::to_string(f));
  return *this;
}

LogInput& LogInput::operator<<(double f) {
  buf.append(std::to_string(f));
  return *this;
}

LogInput& LogInput::operator<<(const void *ptr) {
  std::stringbuf hbuf;
  std::ostream os(&hbuf);
  os << "0x" << std::setfill('0') << std::setw(sizeof(ptr) * 2) << std::hex << (uint64_t)ptr;
  buf.append(hbuf.str());
  return *this;
}


LogInput& LogInput::operator<<(const Vector2f &v) {
  *this << '(' << v.x << ", " << v.y << ')';
  return *this;
}

LogInput& LogInput::operator<<(const Vector3f &v) {
  *this << '(' << v.x << ", " << v.y << ", " << v.z << ')';
  return *this;
}

LogInput& LogInput::operator<<(const Vector4f &v) {
  *this << '(' << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ')';
  return *this;
}

} /* namespace glPortal */
