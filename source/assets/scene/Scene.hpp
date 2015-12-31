#ifndef SCENE_HPP
#define SCENE_HPP

#include <vector>
#include <map>

#include <bullet/btBulletDynamicsCommon.h>
#include <bullet/BulletCollision/CollisionDispatch/btGhostObject.h>

#include <assets/material/Material.hpp>
#include <engine/Camera.hpp>
#include <engine/EntityManager.hpp>
#include <engine/physics/CollisionDispatcher.hpp>

#include "Portal.hpp"
#include "Screen.hpp"
#include "Terminal.hpp"

namespace glPortal {

typedef std::pair<Entity*, Entity*> EntityPair;
class World;

class Scene {
public:
  struct Physics {
    Scene &parent;
    btBroadphaseInterface *broadphase;
    btDefaultCollisionConfiguration *collisionConfiguration;
    CollisionDispatcher *dispatcher;
    btSequentialImpulseConstraintSolver *solver;
    btDiscreteDynamicsWorld *world;
    btGhostPairCallback *gpCallback;
    Physics(Scene &parent);
    ~Physics();
    void setGravity(float x, float y, float z);

    Physics(const Physics&) = delete;
    Physics(Physics&&) = delete;
  } physics;

  World *world;
  Camera camera;
  std::map<int, Material> materials;
  EntityManager entities;
  std::vector<EntityPair> portalPairs;
  std::unique_ptr<Screen> screen;
  std::unique_ptr<Terminal> terminal;
  Entity *player;
  // FIXME: remove us!
  Entity *start;
  Entity *end;

  Scene();
  Scene(Scene&) = delete;
  Scene(Scene&&) = delete;
};

} /* namespace glPortal */

#endif /* SCENE_HPP */
