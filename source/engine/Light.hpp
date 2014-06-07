#ifndef LIGHT_HPP
#define LIGHT_HPP

#include "util/Vector3f.hpp"

namespace glPortal {

class Light {
public:
  Light() : color(1, 1, 1), intensity(1) {}
  Vector3f position;
  Vector3f color;
  float intensity;
};

} /* namespace glPortal */

#endif /* LIGHT_HPP */
