#include "BoxCollider.hpp"

namespace glPortal {

  BoxCollider::BoxCollider(const Vector3f &position, const Vector3f &size) {
    this->position.set(position);
    this->size.set(size);
  }

  bool BoxCollider::collidesWith(const BoxCollider &collider) const {
    if(position.x - size.x/2 < collider.position.x + collider.size.x/2 &&
       position.x + size.x/2 > collider.position.x - collider.size.x/2 &&
       position.y - size.y/2 < collider.position.y + collider.size.y/2 &&
       position.y + size.y/2 > collider.position.y - collider.size.y/2 &&
       position.z - size.z/2 < collider.position.z + collider.size.z/2 &&
       position.z + size.z/2 > collider.position.z - collider.size.z/2) {
      return true;
    }
    return false;
  }
} /* namespace glPortal */
