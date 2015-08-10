#include "Scene.hpp"
#include <engine/component/Transform.hpp>
#include <engine/component/Health.hpp>
#include <engine/component/SoundSource.hpp>
#include <engine/component/SoundListener.hpp>
#include <engine/component/RigidBody.hpp>
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
  world(nullptr),
  start(nullptr),
  end(nullptr),
  entities(*this) {
  player = &entities.create();
  player->addComponent<Transform>();
  player->getComponent<Transform>().position = Vector3f(2.5, 1, 5);
  player->addComponent<Health>();
  player->addComponent<SoundSource>();
  player->addComponent<SoundListener>();
  player->addComponent<RigidBody>(1, std::make_shared<btCapsuleShape>(.4, 1));
  player->addComponent<PlayerMotion>();
}

} /* namespace glPortal */
