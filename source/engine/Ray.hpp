#ifndef RAY_HPP
#define RAY_HPP

#include <engine/core/math/Vector3f.hpp>

namespace glPortal {
class Entity;

class Ray {
public:
  Ray(Vector3f origin, Vector3f direction) {
    this->origin = origin;
    this->direction = direction;
  }

  Vector3f origin;
  Vector3f direction;

  bool collides(Entity e, float* tNear, float* tFar);
};

} /* namespace glPortal */

#endif /* RAY_HPP */
