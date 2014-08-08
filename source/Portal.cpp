#include "Portal.hpp"

#include "engine/BoxCollider.hpp"

namespace glPortal {
  const int Portal::PORTAL_RANGE = 1000;
  const Vector3f Portal::BLUE_COLOR = Vector3f(0.33, 0.57, 1);
  const Vector3f Portal::ORANGE_COLOR = Vector3f(1, 0.76, 0.33);

  Vector3f Portal::getDirection() {
    if(rotation.x == 0) {
      if(rotation.y == 90) { return Vector3f(-1, 0, 0); }
      if(rotation.y == -90) { return Vector3f(1, 0, 0); }
      if(rotation.y == 0) { return Vector3f(0, 0, -1); }
      if(rotation.y == 180) { return Vector3f(0, 0, 1); }
    }
    if(rotation.x == -90) {
      return Vector3f(0, -1, 0);
    }
    if(rotation.x == 90) {
      return Vector3f(0, 1, 0);
    }
    return Vector3f(0, 0, -1);
  }

  bool Portal::throughPortal(Vector3f v) {
    if(rotation.y == 90) {
      if(v.x > position.x && v.x < position.x + 0.2f &&
         v.y > position.y - 1.0f && v.y < position.y + 1.0f &&
         v.z > position.z - 0.5f && v.z < position.z + 0.5f) {
        return true;
      }
    }
    if(rotation.y == -90) {
      if(v.x < position.x && v.x > position.x - 0.2f &&
         v.y > position.y - 1.0f && v.y < position.y + 1.0f &&
         v.z > position.z - 0.5f && v.z < position.z + 0.5f) {
        return true;
      }
    }
    if(rotation.y == 0) {
      if(v.x > position.x - 0.5f && v.x < position.x + 0.5f &&
         v.y > position.y - 1.0f && v.y < position.y + 1.0f &&
         v.z > position.z  && v.z < position.z + 0.2f) {
        return true;
      }
    }
    if(rotation.y == 180) {
      if(v.x > position.x - 0.5f && v.x < position.x + 0.5f &&
         v.y > position.y - 1.0f && v.y < position.y + 1.0f &&
         v.z < position.z  && v.z > position.z - 0.2f) {
        return true;
      }
    }
    return false;
  }

  bool Portal::inPortal(BoxCollider collider) {
    if(collider.position.x - collider.size.x/2 > position.x - scale.x/2 &&
       collider.position.x + collider.size.x/2 < position.x + scale.x/2 &&
       collider.position.y - collider.size.y/2 > position.y - scale.y/2 &&
       collider.position.y + collider.size.y/2 < position.y + scale.y/2 &&
       collider.position.z - collider.size.z/2 > position.z - scale.z/2 &&
       collider.position.z + collider.size.z/2 < position.z + scale.z/2) {
      return true;
    }
    return false;
  }
} /* namespace glPortal */
