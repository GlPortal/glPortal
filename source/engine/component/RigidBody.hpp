#ifndef COMPONENT_RIGIDBODY_HPP
#define COMPONENT_RIGIDBODY_HPP

#include "Component.hpp"
#include <memory>
#include <bullet/btBulletDynamicsCommon.h>
#include <engine/Entity.hpp>
#include <engine/EntityManager.hpp>
#include <assets/scene/Scene.hpp>
#include "Transform.hpp"

namespace glPortal {

class RigidBody : public Component {
public:
  std::shared_ptr<btCollisionShape> shape;
  btRigidBody *body;

  RigidBody(Entity &ent, float mass, const std::shared_ptr<btCollisionShape> &collisionshape) :
    Component(ent), shape(collisionshape) {
    if (not entity.hasComponent<Transform>()) {
      entity.addComponent<Transform>();
    }
    Transform &tform = entity.getComponent<Transform>();
    btDefaultMotionState motionState(btTransform(tform.quat, tform.position));
    btRigidBody::btRigidBodyConstructionInfo ci(mass, &motionState, shape.get());
    body = new btRigidBody(ci);
    entity.manager.scene.physics.world->addRigidBody(body);
  }

  ~RigidBody() {
    entity.manager.scene.physics.world->removeRigidBody(body);
    delete body;
  }
};

} /* namespace glPortal */

#endif /* COMPONENT_RIGIDBODY_HPP */
