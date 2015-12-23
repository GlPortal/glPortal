#include "RigidBody.hpp"

namespace glPortal {

RigidBody::RigidBody(Entity &ent, float mass,
  const std::shared_ptr<btCollisionShape> &collisionshape) :
  Component(ent), shape(collisionshape) {
  if (not entity.hasComponent<Transform>()) {
    entity.addComponent<Transform>();
  }
  Transform &tform = entity.getComponent<Transform>();
  motionState.setWorldTransform(btTransform(tform.getOrientation(), tform.getPosition()));
  btVector3 localInertia;
  collisionshape->calculateLocalInertia(mass, localInertia);
  btRigidBody::btRigidBodyConstructionInfo ci(mass, &motionState, shape.get(), localInertia);
  body = new btRigidBody(ci);
  body->setUserPointer(&entity);
  entity.manager.scene.physics.world->addRigidBody(body);
}

RigidBody::~RigidBody() {
  entity.manager.scene.physics.world->removeRigidBody(body);
  delete body;
}

} /* namespace glPortal */