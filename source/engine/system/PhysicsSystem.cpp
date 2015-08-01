#include "PhysicsSystem.hpp"

namespace glPortal {

void PhysicsSystem::setScene(Scene *scene) {
  this->scene = scene;
}

void PhysicsSystem::update(float dtime) {
  scene->physics.world->stepSimulation(dtime, 10);
}

} /* namespace glPortal */
