#ifndef TEXT_HPP
#define TEXT_HPP

#include <string>
#include <util/Vector3f.hpp>
#include <engine/Font.hpp>
#include <engine/loader/FontLoader.hpp>

namespace glPortal {

class Text {
public:
  Text(std::string text, Vector3f position);
  Vector3f position;
  std::string text;
  Font font;
  int size;
};

} /* namespace glPortal */

#endif /* TEXT_HPP */
