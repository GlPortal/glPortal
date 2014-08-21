#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <GL/glew.h>

namespace glPortal {

class Texture {
public:
  GLuint handle;
  int width;
  int height;
  float xTiling = 1;
  float yTiling = 1;
};

} /* namespace glPortal */

#endif /* TEXTURE_HPP */
