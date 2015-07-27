#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <epoxy/gl.h>

namespace glPortal {

struct Texture {
  GLuint handle = 0xBAD7E96;
  int width;
  int height;
};

} /* namespace glPortal */

#endif /* TEXTURE_HPP */
