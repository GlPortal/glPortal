#ifndef COMPONENT_RIGIDBODY_HPP
#define COMPONENT_RIGIDBODY_HPP

#include "Component.hpp"
#include <memory>
#include <bullet/BulletCollision/CollisionShapes/btCollisionShape.h>
#include <bullet/BulletDynamics/Dynamics/btRigidBody.h>
#include <bullet/LinearMath/btDefaultMotionState.h>
#include <engine/Entity.hpp>
#include <engine/EntityManager.hpp>
#include <assets/scene/Scene.hpp>
#include "Transform.hpp"

namespace glPortal {

class RigidBody : public Component {
public:
  std::shared_ptr<btCollisionShape> shape;
  btDefaultMotionState motionState;
  btRigidBody *body;

  RigidBody(Entity &ent, float mass, const std::shared_ptr<btCollisionShape> &collisionshape);
  ~RigidBody();
};

} /* namespace glPortal */

#endif /* COMPONENT_RIGIDBODY_HPP */
