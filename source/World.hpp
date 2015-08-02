#ifndef WORLD_HPP
#define WORLD_HPP

#include <string>
#include <vector>
#include <array>
#include <random>

#include <assets/scene/Scene.hpp>
#include <engine/env/ConfigFileParser.hpp>
#include <engine/env/Config.hpp>
#include <engine/system/PhysicsSystem.hpp>

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

  void update();
  void loadScene(const std::string &path);
  bool collidesWithWalls(const BoxCollider &collider) const;
  void shootPortal(int button);
  bool collides(const Vector3f &ro, const Vector3f &rd, const Entity &e, float *tNear, float *tFar);
  void render();
  Entity& getPlayer();
  void hidePortals();
  static float gravity;
  bool isEditorShown;
  std::string currentScenePath;
private:
  uint32_t lastUpdateTime;
  Renderer *renderer;
  Editor *editor;
  Scene *scene;
  // FIXME: now, aggregating all of this in here is more like patchwork rather than coding
  PhysicsSystem phys;
  std::mt19937 generator;
  std::vector<std::string> mapList;
  unsigned int currentLevel = 0;
  ConfigFileParser *config;
};

} /* namespace glPortal */

#endif /* WORLD_HPP */
