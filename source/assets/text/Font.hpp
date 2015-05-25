#ifndef FONT_HPP
#define FONT_HPP

#include <assets/text/Letter.hpp>
#include <map>

namespace glPortal {

class Font {
public:
  int num_chars;
  float size;

  const Letter& getLetter(int index) const {
    return letters.at(index);
  }

  std::map<int, Letter> letters;
};

} /* namespace glPortal */

#endif /* FONT_HPP */
