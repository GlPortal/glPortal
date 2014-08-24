#include <engine/BoxCollider.hpp>
#include <Portal.hpp>
#include <cstdio>

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

  bool Portal::throughPortal(BoxCollider collider) {
    if(rotation.x == 0) {
      if(rotation.y == 90) {
        if(collider.position.x > position.x &&
           collider.position.x + collider.size.x/2 < position.x + scale.x &&
           collider.position.y - collider.size.y/2 > position.y - scale.y/2 &&
           collider.position.y + collider.size.y/2 < position.y + scale.y/2 &&
           collider.position.z - collider.size.z/2 > position.z - scale.z/2 &&
           collider.position.z + collider.size.z/2 < position.z + scale.z/2) {
          return true;
        }
      }
      if(rotation.y == -90) {
        if(collider.position.x < position.x &&
           collider.position.x - collider.size.x/2 > position.x - scale.x &&
           collider.position.y - collider.size.y/2 > position.y - scale.y/2 &&
           collider.position.y + collider.size.y/2 < position.y + scale.y/2 &&
           collider.position.z - collider.size.z/2 > position.z - scale.z/2 &&
           collider.position.z + collider.size.z/2 < position.z + scale.z/2) {
          return true;
        }
      }
      if(rotation.y == 0) {
        if(collider.position.x - collider.size.x/2 > position.x - scale.x/2 &&
           collider.position.x + collider.size.x/2 < position.x + scale.x/2 &&
           collider.position.y - collider.size.y/2 > position.y - scale.y/2 &&
           collider.position.y + collider.size.y/2 < position.y + scale.y/2 &&
           collider.position.z > position.z &&
           collider.position.z + collider.size.z/2 < position.z + scale.z) {
          return true;
        }
      }
      if(rotation.y == 180) {
        if(collider.position.x - collider.size.x/2 > position.x - scale.x/2 &&
           collider.position.x + collider.size.x/2 < position.x + scale.x/2 &&
           collider.position.y - collider.size.y/2 > position.y - scale.y/2 &&
           collider.position.y + collider.size.y/2 < position.y + scale.y/2 &&
           collider.position.z < position.z &&
           collider.position.z - collider.size.z/2 > position.z - scale.z) {
          return true;
        }
      }
    } else {
      if(rotation.x == -90) {
        if(collider.position.x - collider.size.x/2 > position.x - scale.x/2 &&
           collider.position.x + collider.size.x/2 < position.x + scale.x/2 &&
           collider.position.y > position.y &&
           collider.position.y + collider.size.y/2 < position.y + scale.y &&
           collider.position.z - collider.size.z/2 > position.z - scale.z/2 &&
           collider.position.z + collider.size.z/2 < position.z + scale.z/2) {
          return true;
        }
      }
      if(rotation.x == 90) {
        if(collider.position.x - collider.size.x/2 > position.x - scale.x/2 &&
           collider.position.x + collider.size.x/2 < position.x + scale.x/2 &&
           collider.position.y < position.y &&
           collider.position.y - collider.size.y/2 > position.y - scale.y &&
           collider.position.z - collider.size.z/2 > position.z - scale.z/2 &&
           collider.position.z + collider.size.z/2 < position.z + scale.z/2) {
          return true;
        }
      }
    }
    return false;
  }

  bool Portal::inPortal(BoxCollider collider) {
    if(!open) {
      return false;
    }
    if(rotation.x == 0) {
      if(rotation.y == 90) {
        if(collider.position.x + collider.size.x/2 > position.x &&
           collider.position.x + collider.size.x/2 < position.x + scale.x &&
           collider.position.y - collider.size.y/2 > position.y - scale.y/2 &&
           collider.position.y + collider.size.y/2 < position.y + scale.y/2 &&
           collider.position.z - collider.size.z/2 > position.z - scale.z/2 &&
           collider.position.z + collider.size.z/2 < position.z + scale.z/2) {
          return true;
        }
      }
      if(rotation.y == -90) {
        if(collider.position.x - collider.size.x/2  < position.x &&
           collider.position.x - collider.size.x/2 > position.x - scale.x &&
           collider.position.y - collider.size.y/2 > position.y - scale.y/2 &&
           collider.position.y + collider.size.y/2 < position.y + scale.y/2 &&
           collider.position.z - collider.size.z/2 > position.z - scale.z/2 &&
           collider.position.z + collider.size.z/2 < position.z + scale.z/2) {
          return true;
        }
      }
      if(rotation.y == 0) {
        if(collider.position.x - collider.size.x/2 > position.x - scale.x/2 &&
           collider.position.x + collider.size.x/2 < position.x + scale.x/2 &&
           collider.position.y - collider.size.y/2 > position.y - scale.y/2 &&
           collider.position.y + collider.size.y/2 < position.y + scale.y/2 &&
           collider.position.z + collider.size.z/2 > position.z &&
           collider.position.z + collider.size.z/2 < position.z + scale.z) {
          return true;
        }
      }
      if(rotation.y == 180) {
        if(collider.position.x - collider.size.x/2 > position.x - scale.x/2 &&
           collider.position.x + collider.size.x/2 < position.x + scale.x/2 &&
           collider.position.y - collider.size.y/2 > position.y - scale.y/2 &&
           collider.position.y + collider.size.y/2 < position.y + scale.y/2 &&
           collider.position.z - collider.size.z/2 < position.z &&
           collider.position.z - collider.size.z/2 > position.z - scale.z) {
          return true;
        }
      }
    } else {
      if(rotation.x == -90) {
        if(collider.position.x - collider.size.x/2 > position.x - scale.x/2 &&
           collider.position.x + collider.size.x/2 < position.x + scale.x/2 &&
           collider.position.y + collider.size.y/2 > position.y &&
           collider.position.y + collider.size.y/2 < position.y + scale.y &&
           collider.position.z - collider.size.z/2 > position.z - scale.z/2 &&
           collider.position.z + collider.size.z/2 < position.z + scale.z/2) {
          return true;
        }
      }
      if(rotation.x == 90) {
        if(collider.position.x - collider.size.x/2 > position.x - scale.x/2 &&
           collider.position.x + collider.size.x/2 < position.x + scale.x/2 &&
           collider.position.y - collider.size.y/2 < position.y &&
           collider.position.y - collider.size.y/2 > position.y - scale.y &&
           collider.position.z - collider.size.z/2 > position.z - scale.z/2 &&
           collider.position.z + collider.size.z/2 < position.z + scale.z/2) {
          return true;
        }
      }
    }
    return false;
  }
} /* namespace glPortal */
