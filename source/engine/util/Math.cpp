#include "Math.hpp"

namespace glPortal {
  const float Math::PI_RND=3.1415926535897932384626433832795028841971693993751058209749445923078164062862089;
  
  float Math::toDegrees(float radians) {
    return radians * (180 / PI_RND);
  }

  float Math::toRadians(float degrees) {
    return degrees * (PI_RND / 180);
  }

} /* namespace glPortal */
