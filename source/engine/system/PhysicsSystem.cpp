#include "PhysicsSystem.hpp"
#include <engine/component/MeshDrawable.hpp>
#include <engine/component/Transform.hpp>
#include <engine/component/RigidBody.hpp>
#include <assets/material/MaterialLoader.hpp>
#include <assets/model/MeshLoader.hpp>

namespace glPortal {

void PhysicsSystem::setScene(Scene *scene) {
  this->scene = scene;
}

void PhysicsSystem::update(float dtime) {
  // TESTING
  static bool spawned = false;
  static float time = 0;
  if (not spawned) {
    if (time < 1) {
      time += dtime;
    } else {
      Entity &e = scene->entities.create();
      Transform &t = e.addComponent<Transform>();
      t.position.x = 10; t.position.y = 6; t.position.z = 5;
      MeshDrawable &m = e.addComponent<MeshDrawable>();
      m.material = MaterialLoader::fromTexture("HumanToken.png");
      m.mesh = MeshLoader::getMesh("Cube.obj");
      e.addComponent<RigidBody>(1, std::make_shared<btBoxShape>(btVector3(.5, .5, .5)));
      spawned = true;
    }
  }
  scene->physics.world->stepSimulation(dtime, 10);
  for (Entity &e : scene->entities) {
    if (e.hasComponent<RigidBody>()) {
      RigidBody &b = e.getComponent<RigidBody>();
      if (not b.body->isStaticObject()) {
        Transform &t = e.getComponent<Transform>();
        btTransform trans;
        b.body->getMotionState()->getWorldTransform(trans);
        btVector3 &origin = trans.getOrigin();
        t.position.x = origin.getX();
        t.position.y = origin.getY();
        t.position.z = origin.getZ();
      }
    }
  }
}

} /* namespace glPortal */
