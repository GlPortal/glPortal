#ifndef COMPONENT_RIGIDBODY_HPP
#define COMPONENT_RIGIDBODY_HPP

#include "Component.hpp"
#include <bullet/btBulletDynamicsCommon.h>
#include <engine/Entity.hpp>
#include <engine/EntityManager.hpp>
#include <assets/scene/Scene.hpp>

namespace glPortal {

class RigidBody : public Component {
public:
  btCollisionShape *shape;
  btDefaultMotionState *groundMotionState;
  btRigidBody::btRigidBodyConstructionInfo *ci;
  btRigidBody *body;

  RigidBody(Entity &ent) :
    Component(ent) {
    entity.manager.scene.physics.world->addRigidBody(body);
  }

  ~RigidBody() {
    entity.manager.scene.physics.world->removeRigidBody(body);
  }
};

} /* namespace glPortal */

#endif /* COMPONENT_RIGIDBODY_HPP */
