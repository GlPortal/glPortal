#ifndef SCENE_HPP
#define SCENE_HPP

#include <vector>
#include <map>

#include <bullet/btBulletDynamicsCommon.h>
#include <bullet/BulletCollision/CollisionDispatch/btGhostObject.h>

#include <assets/gui/GUIButton.hpp>
#include <assets/material/Material.hpp>
#include <engine/Camera.hpp>
#include <engine/EntityManager.hpp>

#include "Portal.hpp"

namespace glPortal {

typedef std::pair<Entity*, Entity*> EntityPair;
class World;

class Scene {
public:
  struct Physics {
    Scene &parent;
    btBroadphaseInterface *broadphase;
    btDefaultCollisionConfiguration *collisionConfiguration;
    btCollisionDispatcher *dispatcher;
    btSequentialImpulseConstraintSolver *solver;
    btDiscreteDynamicsWorld *world;
    btGhostPairCallback *gpCallback;
    Physics(Scene &parent);
    void setGravity(float x, float y, float z);
    ~Physics();
  } physics;

  World *world;

  Entity *player;
  Camera camera;
  std::map<int, Material> materials;
  EntityManager entities;
  std::vector<EntityPair> portalPairs;
  // FIXME: remove us!
  Entity *start;
  Entity *end;

  Scene();
  Scene(Scene&) = delete;
  Scene(Scene&&) = delete;
};

} /* namespace glPortal */

#endif /* SCENE_HPP */
