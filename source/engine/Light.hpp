#ifndef LIGHT_HPP
#define LIGHT_HPP

#include <util/math/Vector3f.hpp>

namespace glPortal {

class Light {
public:
  Light() :
      color(1, 1, 1), constantAtt(0.6), linearAtt(0.05), quadraticAtt(0.0008) {
  }
  Vector3f position;
  Vector3f color;
  float constantAtt;
  float linearAtt;
  float quadraticAtt;
};

} /* namespace glPortal */

#endif /* LIGHT_HPP */
