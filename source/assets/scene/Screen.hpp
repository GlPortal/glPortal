#ifndef SCREEN_HPP
#define SCREEN_HPP
#include <string>
#include <engine/core/math/Vector2f.hpp>

namespace glPortal {

class Screen {
public:
  Screen(std::string title, std::string text);
  std::string title;
  std::string text;
};

} /* namespace glPortal */

#endif /* SCREEN_HPP */
