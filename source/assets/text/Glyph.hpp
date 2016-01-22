#ifndef GLYPH_HPP
#define GLYPH_HPP

#include <assets/model/Mesh.hpp>

namespace glPortal {

class Glyph {
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

#endif /* GLYPH_HPP */
