#ifndef COMPONENT_RIGIDBODY_HPP
#define COMPONENT_RIGIDBODY_HPP

#include "Component.hpp"
#include <bullet/btBulletDynamicsCommon.h>

namespace glPortal {

class RigidBody : public Component {
public:
  btCollisionShape *shape;
  btDefaultMotionState *groundMotionState;
  btRigidBody::btRigidBodyConstructionInfo *ci;
  btRigidBody *body;

  RigidBody(Entity &ent) :
    Component(ent) {
  }
};

} /* namespace glPortal */

#endif /* COMPONENT_RIGIDBODY_HPP */
