#ifndef TEXTURE_HPP
#define TEXTURE_HPP

namespace glPortal {

class Texture {
public:
  int handle;
  int width;
  int height;
  float xTiling = 1;
  float yTiling = 1;
};

} /* namespace glPortal */

#endif /* TEXTURE_HPP */
