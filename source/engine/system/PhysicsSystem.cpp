#include "PhysicsSystem.hpp"
#include <engine/component/MeshDrawable.hpp>
#include <engine/component/Transform.hpp>
#include <engine/component/RigidBody.hpp>
#include <assets/material/MaterialLoader.hpp>
#include <engine/physics/Uncollider.hpp>
#include <assets/model/MeshLoader.hpp>

namespace glPortal {

void PhysicsSystem::setScene(Scene *scene) {
  scene->physics.world->getPairCache()->setOverlapFilterCallback(nullptr);
  delete filterCallback;

  this->scene = scene;
  filterCallback = new Uncollider(*scene);
  //scene->physics.world->getPairCache()->setOverlapFilterCallback(filterCallback);
  scene->physics.dispatcher->setNearCallback(Uncollider::nearCallback);
}

PhysicsSystem::PhysicsSystem() :
  filterCallback(nullptr) {
}

PhysicsSystem::~PhysicsSystem() {
  delete filterCallback;
}

void PhysicsSystem::update(float dtime) {
#if 0
  // TESTING
  static bool spawned = false;
  static float time = 0;
  if (not spawned) {
    if (time < 1) {
      time += dtime;
    } else {
      Vector3f posz[3] = {Vector3f(10.6, 6, 5), Vector3f(10, 5, 5), Vector3f(10, 4, 5.6)};
      for (const Vector3f &pos : posz) {
        Entity &e = scene->entities.create();
        Transform &t = e.addComponent<Transform>();
        t.setPosition(pos);
        t.setOrientation(Quaternion().fromAero(0, rad(45), 0));
        MeshDrawable &m = e.addComponent<MeshDrawable>();
        m.material = MaterialLoader::getMaterial("boxes/dev00");
        m.mesh = MeshLoader::getMesh("Cube.obj");
        e.addComponent<RigidBody>(1, std::make_shared<btBoxShape>(btVector3(.5, .5, .5)));
      }
      spawned = true;
    }
  }
#endif
  filterCallback->beforePhysicsStep();
  scene->physics.world->stepSimulation(dtime, 10);
  for (Entity &e : scene->entities) {
    if (e.hasComponent<RigidBody>()) {
      RigidBody &b = e.getComponent<RigidBody>();
      if (not b.body->isStaticObject()) {
        Transform &t = e.getComponent<Transform>();
        btTransform btTform;
        b.body->getMotionState()->getWorldTransform(btTform);
        t.privSetPosition(btTform.getOrigin());
        t.privSetOrientation(btTform.getRotation());
      }
    }
  }
}

} /* namespace glPortal */
