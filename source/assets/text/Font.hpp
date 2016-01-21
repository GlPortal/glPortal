#ifndef FONT_HPP
#define FONT_HPP

#include <map>
#include <string>
#include <assets/text/Glyph.hpp>

namespace glPortal {

class Font {
public:
  int num_chars;
  float size;

  const Glyph& getGlyph(int index) const {
    return letters.at(index);
  }

  const int getStringLength(std::string string) {
    int length = 0;
    const char *array = string.c_str();
    for (unsigned int i = 0; i < string.length(); i++) {
      char c = array[i];

      const Glyph &letter = getGlyph(c);

      length = length + (letter.width * this->size);
    }
    return length;
  }

  std::map<int, Glyph> letters;
};

} /* namespace glPortal */

#endif /* FONT_HPP */
