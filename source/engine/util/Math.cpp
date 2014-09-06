#include <engine/util/Math.hpp>
#include <engine/util/Vector3f.hpp>
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
    Vector3f direction(cos(Math::toRadians(v.x)) * -sin(Math::toRadians(v.y)),
                       sin(Math::toRadians(v.x)),
                       -cos(Math::toRadians(v.x)) * cos(Math::toRadians(v.y)));
    return direction;
  }

  Vector3f Math::toVector(Vector3f direction) {
    //TODO
    return Vector3f(1, 1, 1);
  }
} /* namespace glPortal */
