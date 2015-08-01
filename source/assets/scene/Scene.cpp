#include "Scene.hpp"
#include <engine/component/Transform.hpp>
#include <engine/component/Health.hpp>
#include <engine/component/SoundSource.hpp>
#include <engine/component/SoundListener.hpp>
#include "../../PlayerMotion.hpp"

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
  physics(*this),
  entities(*this) {
  player = &entities.create();
  player->addComponent<Transform>();
  player->addComponent<PlayerMotion>();
  player->addComponent<Health>();
  player->addComponent<SoundSource>();
  player->addComponent<SoundListener>();
}

} /* namespace glPortal */
