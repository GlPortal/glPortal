#include "Scene.hpp"

namespace glPortal {

Scene::Physics::Physics(Scene &parent) :
  parent(parent),
  broadphase(new btDbvtBroadphase()),
  collisionConfiguration(new btDefaultCollisionConfiguration()),
  dispatcher(new btCollisionDispatcher(collisionConfiguration)),
  solver(new btSequentialImpulseConstraintSolver),
  world(new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration)) {
}

void Scene::Physics::setGravity(float x, float y, float z) {
  world->setGravity(btVector3(x, y, z));
}

Scene::Physics::~Physics() {
  delete world;
  delete solver;
  delete dispatcher;
  delete collisionConfiguration;
  delete broadphase;
}

Scene::Scene() : 
  physics(*this) {
}

} /* namespace glPortal */
