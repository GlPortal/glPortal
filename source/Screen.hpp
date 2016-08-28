#ifndef SCREEN_HPP
#define SCREEN_HPP

#include <string>

#include <radix/core/math/Vector4f.hpp>

namespace glPortal {

class Screen {
public:
  Screen(std::string title, std::string text) :
    title(title),
    text(text), enabled(false), alpha(0) {
  }
  std::string title;
  std::string text;
  bool enabled;
  float alpha;
  radix::Vector4f backgroundColor;
  radix::Vector4f textColor;
};

} /* namespace glPortal */

#endif /* SCREEN_HPP */
