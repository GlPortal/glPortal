#ifndef SCREEN_HPP
#define SCREEN_HPP

#include <string>
#include <engine/core/math/Vector4f.hpp>

namespace glPortal {

class Screen {
public:
  Screen(std::string title, std::string text);
  std::string title;
  std::string text;
  bool enabled;
  float alpha;
  Vector4f backgroundColor;
  Vector4f textColor;
};

} /* namespace glPortal */

#endif /* SCREEN_HPP */
