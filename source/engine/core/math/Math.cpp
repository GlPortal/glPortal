#include "Math.hpp"
#include <engine/core/math/Matrix4f.hpp>
#include <engine/core/math/Vector3f.hpp>
#include <cmath>

namespace glPortal {
  const float Math::PI_RND=3.1415926535897932384626433832795028841971693993751058209749445923078164062862089;
  
  float Math::toDegrees(float radians) {
    return radians * (180 / PI_RND);
  }

  float Math::toRadians(float degrees) {
    return degrees * (PI_RND / 180);
  }

  Vector3f Math::toDirection(Vector3f v) {
    Matrix4f m;
    m.rotate(v);

    Vector3f direction = m.transform(Vector3f(0, 0, -1));
    return direction;
  }

  Vector3f Math::toEuler(Vector3f direction) {
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
