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
  static const int NOISE_FADE_DELAY;
  static const int OPEN_ANIM_DURATION;

  Portal() : open(false) {}
  Vector3f getDirection();
  bool throughPortal(const BoxCollider &collider) const;
  bool inPortal(const BoxCollider &collider) const;
  void placeOnWall(const Vector3f &launchPos, const BoxCollider &wall, const Vector3f &point);

  Vector3f getScaleMult() const;

  Vector3f direction, color;
  bool open;
  Material maskTex;
  uint32_t openSince;
};

} /* namespace glPortal */

#endif /* PORTAL_HPP */
