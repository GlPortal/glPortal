#ifndef FONT_HPP
#define FONT_HPP

#include <engine/Letter.hpp>
#include <map>

namespace glPortal {

class Font {
public:
  int num_chars;

  Letter getLetter(int index) {
    return letters.at(index);
  }

  std::map<int, Letter> letters;
};

} /* namespace glPortal */

#endif /* FONT_HPP */
