#ifndef PORTAL_HPP
#define PORTAL_HPP

#include <engine/component/Component.hpp>
#include <engine/core/math/Vector3f.hpp>
#include <engine/BoxCollider.hpp>
#include <assets/material/Material.hpp>
#include <assets/model/Mesh.hpp>

namespace glPortal {

class Portal : public Component {
public:
  static const int PORTAL_RANGE;
  static const Vector3f BLUE_COLOR;
  static const Vector3f ORANGE_COLOR;
  static const int NOISE_FADE_DELAY;
  static const int OPEN_ANIM_DURATION;
  static const float SURFACE_OFFSET;

  Portal(Entity &ent);
  Vector3f getDirection() const;
  bool throughPortal(const BoxCollider &collider) const;
  bool inPortal(const BoxCollider &collider) const;
  void placeOnWall(const Vector3f &launchPos, const BoxCollider &wall, const Vector3f &point);

  Vector3f getScaleMult() const;

  Material overlayTex, maskTex;
  uint32_t openSince;
  Mesh overlayMesh, stencilMesh;
  Vector3f direction, color;
  bool open;
};

} /* namespace glPortal */

#endif /* PORTAL_HPP */
