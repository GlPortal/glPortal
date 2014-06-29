#ifndef LIGHT_HPP
#define LIGHT_HPP

#include "util/Vector3f.hpp"

namespace glPortal {

class Light {
public:
  Light() :
      color(1, 1, 1), constantAtt(0.2), linearAtt(0.2), quadraticAtt(0.00008) {
  }
  Vector3f position;
  Vector3f color;
  float constantAtt;
  float linearAtt;
  float quadraticAtt;
};

} /* namespace glPortal */

#endif /* LIGHT_HPP */
