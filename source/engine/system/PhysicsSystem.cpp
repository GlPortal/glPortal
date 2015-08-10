#include "PhysicsSystem.hpp"
#include <engine/component/MeshDrawable.hpp>
#include <engine/component/Transform.hpp>
#include <engine/component/RigidBody.hpp>
#include <assets/material/MaterialLoader.hpp>
#include <assets/model/MeshLoader.hpp>
#include "../../PortalFilterCallback.hpp"

namespace glPortal {

void PhysicsSystem::setScene(Scene *scene) {
  scene->physics.world->getPairCache()->setOverlapFilterCallback(nullptr);
  delete filterCallback;

  this->scene = scene;
  filterCallback = new PortalFilterCallback(*scene);
  scene->physics.world->getPairCache()->setOverlapFilterCallback(filterCallback);
  //scene->physics.dispatcher->setNearCallback(PortalFilterCallback::nearCallback);
}

PhysicsSystem::PhysicsSystem() : 
  filterCallback(nullptr) {
}

PhysicsSystem::~PhysicsSystem() {
  delete filterCallback;
}

void PhysicsSystem::update(float dtime) {
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
        t.position = pos;
        t.orientation.setFromEuler(0, rad(45), 0);
        MeshDrawable &m = e.addComponent<MeshDrawable>();
        m.material = MaterialLoader::fromTexture("HumanToken.png");
        m.mesh = MeshLoader::getMesh("Cube.obj");
        e.addComponent<RigidBody>(1, std::make_shared<btBoxShape>(btVector3(.5, .5, .5)));
      }
      spawned = true;
    }
  }
  filterCallback->beforePhysicsStep();
  scene->physics.world->stepSimulation(dtime, 10);
  for (Entity &e : scene->entities) {
    if (e.hasComponent<RigidBody>()) {
      RigidBody &b = e.getComponent<RigidBody>();
      if (not b.body->isStaticObject()) {
        Transform &t = e.getComponent<Transform>();
        btTransform btTform;
        b.body->getMotionState()->getWorldTransform(btTform);
        t.position = btTform.getOrigin();
        t.orientation = btTform.getRotation();
      }
    }
  }
}

} /* namespace glPortal */
