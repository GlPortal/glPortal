#ifndef RESOURCE_HPP
#define RESOURCE_HPP

#include <GL/glew.h>

namespace glPortal {

class Resource {
public:
  static int loadShaders();
  static int loadShader(const char* filename, GLenum type);
};

} /* namespace glPortal */

#endif /* RESOURCE_HPP */
