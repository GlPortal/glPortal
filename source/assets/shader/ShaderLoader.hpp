#ifndef SHADERLOADER_HPP
#define SHADERLOADER_HPP

#include <GL/glew.h>
#include <string>
#include <map>

#include <assets/shader/Shader.hpp>

namespace glPortal {

class ShaderLoader {
public:
  static Shader getShader(std::string path);
  static int loadShader(std::string path, GLenum type);
private:
  static std::map<std::string, Shader> shaderCache;
};

} /* namespace glPortal */

#endif /* SHADERLOADER_HPP */
