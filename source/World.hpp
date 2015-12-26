#ifndef WORLD_HPP
#define WORLD_HPP

#include <array>
#include <random>
#include <stack>
#include <string>
#include <vector>

#include <assets/scene/Scene.hpp>
#include <engine/env/Config.hpp>
#include <engine/core/state/HandleGameFunction.hpp>
#include <engine/physics/PhysicsDebugDraw.hpp>
#include <engine/system/PhysicsSystem.hpp>
#include <engine/system/PlayerSystem.hpp>
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

class World {
public:
  World();
  ~World();
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
  void loadSceneFromPath(const std::string &path);
  void shootPortal(int button);
  Entity& getPlayer();
  void hidePortals();

  static float gravity;
  bool isPhysicsDebugEnabled = false;
  std::string currentScenePath;
  Scene *scene;
  std::stack<HandleGameFunction> stateFunctionStack;
private:
  double gameTime;
  uint32_t lastUpdateTime;
  bool justStarted = true;
  Renderer *renderer;
  Window *window;

  // FIXME: now, aggregating all of this in here is more like patchwork rather than coding
  PhysicsSystem phys;
  PhysicsDebugDraw pdd;
  PortalSystem prtl;
  PlayerSystem plrSys;
  std::mt19937 generator;
  std::vector<std::string> mapList;
  unsigned int currentLevel = 0;
  Config &config;
};

} /* namespace glPortal */

#endif /* WORLD_HPP */
