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

  Player(Entity &ent) :
    Component(ent) {
    if (not entity.hasComponent<Transform>()) {
      entity.addComponent<Transform>();
    }
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

  ~Player() {
    entity.manager.scene.physics.world->removeAction(controller);
    entity.manager.scene.physics.world->removeCollisionObject(obj);
    delete controller;
    delete obj;
  }
};

} /* namespace glPortal */

#endif /* COMPONENT_PLAYER_HPP */
