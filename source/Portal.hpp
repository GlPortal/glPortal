#ifndef PORTAL_HPP
#define PORTAL_HPP

#include <memory>

#include <bullet/BulletCollision/btBulletCollisionCommon.h>

#include <engine/component/Component.hpp>
#include <engine/core/math/Vector3f.hpp>
#include <engine/core/math/Quaternion.hpp>
#include <assets/material/Material.hpp>
#include <assets/model/Mesh.hpp>

namespace glPortal {

class Portal : public Component {
public:
  static const int PORTAL_RANGE;
  static const Vector3f BLUE_COLOR;
  static const Vector3f ORANGE_COLOR;
  static const double NOISE_FADE_DELAY;
  static const double OPEN_ANIM_DURATION;
  static const float SURFACE_OFFSET;

  Portal(Entity &ent);
  ~Portal();
  Vector3f getDirection() const;

  void placeOnWall(const Vector3f &launchPos, const Vector3f &point, const Vector3f &normal);

  Vector3f getScaleMult() const;

  Material overlayTex, maskTex;
  double openSince;
  Mesh overlayMesh, stencilMesh;
  Vector3f direction, color;
  bool open;
  bool isUncolliderActive = false;
  std::unique_ptr<btDefaultMotionState> uncolliderMotionState;
  std::unique_ptr<btCollisionShape> uncolliderShape;
  std::unique_ptr<btCollisionObject> uncollider;
  struct Wrapper {
    std::unique_ptr<btCollisionShape> vertShape, horzShape;
    struct Side {
      std::unique_ptr<btRigidBody> body;
      std::unique_ptr<btDefaultMotionState> motionState;
    } top, right, bottom, left;
  } wrapper;

private:
  void placeWrapperPiece(const Vector3f &p, const Quaternion &o, const Vector3f &s,
    const std::unique_ptr<btCollisionShape> &shape, Wrapper::Side &side, const Vector3f &offset);
};

} /* namespace glPortal */

#endif /* PORTAL_HPP */
