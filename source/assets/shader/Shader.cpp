#include "Shader.hpp"

namespace glPortal {

GLint Shader::uni(const char *name) const {
  return glGetUniformLocation(handle, name);
}

GLint Shader::uni(const std::string &name) const {
  return uni(name.c_str());
}

} /* namespace glPortal */
