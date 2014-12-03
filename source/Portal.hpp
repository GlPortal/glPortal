#ifndef PORTAL_HPP
#define PORTAL_HPP

#include "engine/Entity.hpp"
#include "engine/Light.hpp"
#include <engine/core/math/Vector3f.hpp>

namespace glPortal {
class BoxCollider;

class Portal: public Entity {
public:
  static const int PORTAL_RANGE;
  static const Vector3f BLUE_COLOR;
  static const Vector3f ORANGE_COLOR;

  Portal() : open(false) {}
  Vector3f getDirection();
  bool throughPortal(BoxCollider collider);
  bool inPortal(BoxCollider collider);
  void placeOnWall(BoxCollider wall, Vector3f point);

  Vector3f direction;
  bool open;
};

} /* namespace glPortal */

#endif /* PORTAL_HPP */
