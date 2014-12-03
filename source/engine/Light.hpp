#ifndef LIGHT_HPP
#define LIGHT_HPP

#include <engine/core/math/Vector3f.hpp>

namespace glPortal {

class Light {
public:
  Light() :
      color(1, 1, 1), attenuation(0.2, 0.005, 0.008) {
  }
  Vector3f position;
  Vector3f color;
  Vector3f attenuation;
};

} /* namespace glPortal */

#endif /* LIGHT_HPP */
