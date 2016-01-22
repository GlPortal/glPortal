#ifndef TERMINAL_HPP
#define TERMINAL_HPP

#include <string>
#include <engine/core/math/Vector4f.hpp>

namespace glPortal {

class Terminal {
public:
  Terminal();
  bool enabled;
  Vector4f backgroundColor;
  Vector4f textColor;
};

} /* namespace glPortal */

#endif /* TERMINAL_HPP */
