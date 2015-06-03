#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include <string>
#include <vector>
#include <assets/texture/Texture.hpp>

namespace glPortal {

class Material {
public:
  std::string name, fancyname;
  Texture diffuse, specular, normal, height;
  bool portalable, portalBump;
  float scaleU, scaleV;
  std::string kind;
  std::vector<std::string> tags;

  int tileScale;
};

} /* namespace glPortal */

#endif /* MATERIAL_HPP */
