#ifndef TEXTURE_HPP
#define TEXTURE_HPP

namespace glPortal {

class Texture {
public:
  int handle;
  int width;
  int height;
  float xTiling;
  float yTiling;
};

} /* namespace glPortal */

#endif /* TEXTURE_HPP */
