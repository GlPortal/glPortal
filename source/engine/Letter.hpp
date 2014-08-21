#ifndef LETTER_HPP
#define LETTER_HPP

#include "Mesh.hpp"

namespace glPortal {

class Letter {
public:
  int x;
  int y;
  int width;
  int height;
  float xOffset;
  float yOffset;
  float advance;

  Mesh mesh;
};

} /* namespace glPortal */

#endif /* LETTER_HPP */
