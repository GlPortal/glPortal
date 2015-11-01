#ifndef WORLD_HPP
#define WORLD_HPP

#include <string>
#include <vector>
#include <random>
#include <array>
#include <assets/scene/Scene.hpp>
#include <engine/env/Config.hpp>

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
  void loadSceneFromPath(const std::string &path);
  bool collidesWithWalls(const BoxCollider &collider) const;
  void shootPortal(int button);
  bool collides(const Vector3f &ro, const Vector3f &rd, const Entity &e, float *tNear, float *tFar);
  void render();
  Entity& getPlayer();
  void hidePortals();
  bool isEditorShown;
  std::string currentScenePath;
  static float gravity;
  Scene *scene;
private:
  double gameTime;
  uint32_t lastUpdateTime;
  bool wasF5Down = false;
  bool wasTabDown = false;
  Renderer *renderer;
  Editor *editor;
  std::mt19937 generator;
  std::vector<std::string> mapList;
  unsigned int currentLevel = 0;
  Config &config;
};

} /* namespace glPortal */

#endif /* WORLD_HPP */
