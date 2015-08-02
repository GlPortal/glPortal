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
      Vector3f posz[3] = {Vector3f(10.6, 6, 5), Vector3f(10, 5, 5), Vector3f(10, 4, 5.6)};
      for (const Vector3f &pos : posz) {
        Entity &e = scene->entities.create();
        Transform &t = e.addComponent<Transform>();
        t.position = pos;
        MeshDrawable &m = e.addComponent<MeshDrawable>();
        m.material = MaterialLoader::fromTexture("HumanToken.png");
        m.mesh = MeshLoader::getMesh("Cube.obj");
        e.addComponent<RigidBody>(1, std::make_shared<btBoxShape>(btVector3(.5, .5, .5)));
      }
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
        t.rotation.x = trans.getRotation().getX();
        t.rotation.y = trans.getRotation().getY();
        t.rotation.z = trans.getRotation().getZ();
      }
    }
  }
}

} /* namespace glPortal */
