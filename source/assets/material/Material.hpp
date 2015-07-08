#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include <string>
#include <vector>
#include <assets/texture/Texture.hpp>

namespace glPortal {

class Material {
public:
  static const Material Empty;

  std::string name, fancyname;
  Texture diffuse, specular, normal, height;
  float shininess = 0;
  float scaleU = 1, scaleV = 1;

  bool portalable;
  std::string kind;
  std::vector<std::string> tags;

  int tileScale;
};

} /* namespace glPortal */

#endif /* MATERIAL_HPP */
