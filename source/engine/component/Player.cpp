#include "Player.hpp"

namespace glPortal {

static const Vector3f PLAYER_SIZE(0.5, 1, 0.5);

Player::Player(Entity &ent) :
  Component(ent),
  flying(false),
  noclip(false),
  frozen(false),
  stepCounter(0) {
  if (not entity.hasComponent<Transform>()) {
    entity.addComponent<Transform>();
  }
  entity.getComponent<Transform>().setScale(PLAYER_SIZE);
  obj = new btPairCachingGhostObject;
  Transform &tform = entity.getComponent<Transform>();
  obj->setWorldTransform(btTransform(tform.getOrientation(), tform.getPosition()));
  shape = std::make_shared<btCapsuleShape>(.4, 1);
  obj->setCollisionShape(shape.get());
  obj->setCollisionFlags(btCollisionObject::CF_CHARACTER_OBJECT);
  controller = new KinematicCharacterController(obj, shape.get(), 0.35);
  entity.manager.scene.physics.world->addCollisionObject(obj,
    btBroadphaseProxy::CharacterFilter,
    btBroadphaseProxy::StaticFilter | btBroadphaseProxy::DefaultFilter);
  entity.manager.scene.physics.world->addAction(controller);
}

Player::~Player() {
  entity.manager.scene.physics.world->removeAction(controller);
  entity.manager.scene.physics.world->removeCollisionObject(obj);
  delete controller;
  delete obj;
}

Quaternion Player::getBaseHeadOrientation() const {
  return Quaternion().fromAero(headAngle);
}

Quaternion Player::getHeadOrientation() const {
  return Quaternion().fromAero(headAngle);
}

} /* namespace glPortal */
