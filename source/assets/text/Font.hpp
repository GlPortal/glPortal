#ifndef FONT_HPP
#define FONT_HPP

#include <assets/text/Letter.hpp>
#include <map>
#include <string>

namespace glPortal {

class Font {
public:
  int num_chars;
  float size;

  const Letter& getLetter(int index) const {
    return letters.at(index);
  }

  const int getStringLength(std::string string){
    int length = 0;
    const char *array = string.c_str();
    for (unsigned int i = 0; i < string.length(); i++) {
      char c = array[i];
      
      const Letter &letter = this->getLetter(c);

      length = length + (letter.width * this->size);
    }
    return length;
  }

  std::map<int, Letter> letters;
};

} /* namespace glPortal */

#endif /* FONT_HPP */
