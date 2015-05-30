#ifndef PORTAL_HPP
#define PORTAL_HPP

#include <engine/PhysicsEntity.hpp>
#include <engine/Light.hpp>
#include <engine/core/math/Vector3f.hpp>

namespace glPortal {

class Portal: public PhysicsEntity {
public:
  static const int PORTAL_RANGE;
  static const Vector3f BLUE_COLOR;
  static const Vector3f ORANGE_COLOR;

  Portal() : open(false) {}
  Vector3f getDirection();
  bool throughPortal(const BoxCollider &collider) const;
  bool inPortal(const BoxCollider &collider) const;
  void placeOnWall(const BoxCollider &wall, const Vector3f &point);

  Vector3f direction, color;
  bool open;
  Texture maskTex;
};

} /* namespace glPortal */

#endif /* PORTAL_HPP */
