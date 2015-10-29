#ifndef SCREEN_HPP
#define SCREEN_HPP
#include <string>
#include <engine/core/math/Vector2f.hpp>

namespace glPortal {

class Screen {
public:
  Screen(std::string title, std::string text);
  Vector2f position;
  Vector2f scale;
  std::string title;
  std::string text;
};

} /* namespace glPortal */

#endif /* SCREEN_HPP */
