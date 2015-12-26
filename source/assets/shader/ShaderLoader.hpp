#ifndef SHADERLOADER_HPP
#define SHADERLOADER_HPP

#include <map>
#include <string>
#include <utility>

#include <epoxy/gl.h>

#include <assets/shader/Shader.hpp>

namespace glPortal {

class ShaderLoader {
public:
  static Shader& getShader(const std::string &fragpath, const std::string &vertpath = "");
  static int loadShader(const std::string &path, GLenum type);
private:
  static std::map<std::pair<std::string, std::string>, Shader> shaderCache;
};

} /* namespace glPortal */

#endif /* SHADERLOADER_HPP */
