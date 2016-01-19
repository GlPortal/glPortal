#ifndef THROWABLES_HPP
#define THROWABLES_HPP

#include <stdexcept>
#include <string>

namespace glPortal {
namespace Exception {

class Error : public std::exception {
protected:
  const std::string src, msg;

public:
  Error(const std::string &s, const std::string &m) : src(s), msg(m) {}
  Error(const std::string &s, const char *m) : src(s), msg(m){}
  Error(const char *s, const std::string &m) : src(s), msg(m) {}
  Error(const char *s, const char *m) : src(s), msg(m) {}

  const char* source() const noexcept {
    return src.c_str();
  }
  const char* what() const noexcept {
    return msg.c_str();
  }
};


} /* namespace Exception */
} /* namespace glPortal */

#endif /* THROWABLES_HPP */
