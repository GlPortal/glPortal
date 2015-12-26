#ifndef COMPONENT_PLAYER_HPP
#define COMPONENT_PLAYER_HPP

#include "Component.hpp"
#include <memory>
#include <bullet/btBulletDynamicsCommon.h>
#include <bullet/BulletCollision/CollisionDispatch/btGhostObject.h>
#include <bullet/BulletDynamics/Character/btKinematicCharacterController.h>
#include <engine/Entity.hpp>
#include <engine/EntityManager.hpp>
#include <engine/physics/KinematicCharacterController.hpp>
#include <assets/scene/Scene.hpp>
#include "Transform.hpp"

namespace glPortal {

class Player : public Component {
public:
  std::shared_ptr<btConvexShape> shape;
  btPairCachingGhostObject *obj;
  KinematicCharacterController *controller;

  Vector3f velocity, headAngle;
  bool flying, noclip, frozen;
  float speed;
  float stepCounter;

  Player(Entity&);
  ~Player();

  Quaternion getBaseHeadOrientation() const;
  Quaternion getHeadOrientation() const;
};

} /* namespace glPortal */

#endif /* COMPONENT_PLAYER_HPP */
