#ifndef WORLD_HPP
#define WORLD_HPP

#include <string>
#include <vector>
#include <array>
#include <random>

#include <assets/scene/Scene.hpp>
#include <engine/env/Config.hpp>
#include <engine/physics/PhysicsDebugDraw.hpp>
#include <engine/system/PhysicsSystem.hpp>
#include <engine/system/PortalSystem.hpp>

namespace glPortal {

class Scene;
class Entity;
class Renderer;
class BoxCollider;
class Vector3f;
class SoundManager;

const float GRAVITY = 0.5;

class Window;
class Editor;

class World {
private:
  friend Editor;

public:
  World();
  void create();
  void destroy();

  void setRendererWindow(Window*);

  inline double getTime() const {
    return gameTime;
  }
  void update(double dtime);

  Renderer* getRenderer() const;
  void render(double dtime);

  void loadScene(const std::string &path);
  bool collidesWithWalls(const BoxCollider &collider) const;
  void shootPortal(int button);
  bool collides(const Vector3f &ro, const Vector3f &rd, const Entity &e, float *tNear, float *tFar);
  Entity& getPlayer();
  void hidePortals();
  static float gravity;
  bool isEditorShown;
  bool isPhysicsDebugEnabled = false;
  std::string currentScenePath;
private:
  double gameTime;
  Renderer *renderer;
  Editor *editor;
  Scene *scene;
  // FIXME: now, aggregating all of this in here is more like patchwork rather than coding
  PhysicsSystem phys;
  PhysicsDebugDraw pdd;
  PortalSystem prtl;
  std::mt19937 generator;
  std::vector<std::string> mapList;
  unsigned int currentLevel = 0;
  Config &config;
};

} /* namespace glPortal */

#endif /* WORLD_HPP */
