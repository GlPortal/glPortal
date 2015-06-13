#ifndef RAY_HPP
#define RAY_HPP

#include <engine/core/math/Vector3f.hpp>
#include <engine/BoxCollider.hpp>

namespace glPortal {

class Ray {
public:
  Ray(const Vector3f &origin, const Vector3f &direction) :
    origin(origin), direction(direction) {
  }

  Vector3f origin;
  Vector3f direction;

  bool collides(const BoxCollider &c, float *tNear, float *tFar);
};

} /* namespace glPortal */

#endif /* RAY_HPP */
