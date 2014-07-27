#include "Math.hpp"

namespace glPortal {
  const float Math::PI_RND=3.1415926535897932384626433832795028841971693993751058209749445923078164062862089;
  
  float Math::RAD_TO_DEG(float radius){
    return radius * (180 / Math::PI_RND);
  }

  float Math::DEG_TO_RAD(float degree){
    return degree * (Math::PI_RND / 180);
  }
} /* namespace glPortal */
