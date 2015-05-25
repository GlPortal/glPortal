#include "Math.hpp"
#include <engine/core/math/Matrix4f.hpp>
#include <engine/core/math/Vector3f.hpp>
#include <cmath>

namespace glPortal {
Vector3f Math::toDirection(const Vector3f &v) {
  Matrix4f m;
  m.rotate(v);

  Vector3f direction = m.transform(Vector3f(0, 0, -1));
  return direction;
}

Vector3f Math::toEuler(const Vector3f &direction) {
  Vector3f euler;

  //Pitch
  euler.x = Math::toDegrees(asin(direction.y));

  //Yaw
  if(direction.x <= 0 && direction.z < 0) {
    euler.y = Math::toDegrees(atan(fabs(direction.x) / fabs(direction.z)));
  }
  if(direction.x < 0 && direction.z >= 0) {
    euler.y = Math::toDegrees(atan(fabs(direction.z) / fabs(direction.x))) + 90;
  }
  if(direction.x >= 0 && direction.z > 0) {
    euler.y = Math::toDegrees(atan(fabs(direction.x) / fabs(direction.z))) + 180;
  }
  if(direction.x > 0 && direction.z <= 0) {
    euler.y = Math::toDegrees(atan(fabs(direction.z) / fabs(direction.x))) + 270;
  }

  return euler;
}
} /* namespace glPortal */
