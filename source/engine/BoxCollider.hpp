#ifndef BOXCOLLIDER_HPP
#define BOXCOLLIDER_HPP

#include "util/Vector3f.hpp"

namespace glPortal {

class BoxCollider {
public:
  BoxCollider();
  BoxCollider(Vector3f position, Vector3f size);
  bool collidesWith(BoxCollider collider);

  Vector3f position;
  Vector3f size;
};

} /* namespace glPortal */

#endif /* BOXCOLLIDER_HPP */
