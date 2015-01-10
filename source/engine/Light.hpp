#ifndef LIGHT_HPP
#define LIGHT_HPP

#include <engine/core/math/Vector3f.hpp>

namespace glPortal {

class Light {
public:
  Light() :
      color(1, 1, 1), distance(10), energy(1) {
  }
  Vector3f position;
  Vector3f color;
  float distance;
  float energy;
};

} /* namespace glPortal */

#endif /* LIGHT_HPP */
