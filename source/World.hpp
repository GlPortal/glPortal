#ifndef WORLD_HPP
#define WORLD_HPP

#include <string>
#include <vector>
#include <random>
#include <array>

namespace glPortal {

class Scene;
class Entity;
class Renderer;
class BoxCollider;
class Vector3f;
class SoundManager;

const float GRAVITY = 0.01;
const float FRICTION = 0.01;

const std::array<const std::string,3> MUSIC_PLAYLIST =
{
  "/audio/music/track1.ogg",
  "/audio/music/track2.ogg",
  "/audio/music/track3.ogg"
};

class Player;
class Window;

class World {
public:
  World();
  void create();
  void setRendererWindow(Window *win);
  void destroy();
  void update();
  void loadScene(const std::string &path);
  bool collidesWithWalls(const BoxCollider &collider);
  void shootPortal(int button);
  void render();
  Player* getPlayer();

  static float gravity;
  static float friction;
private:
  Renderer *renderer;
  Scene *scene;
  std::mt19937 generator;
  std::vector<std::string> mapList;
  unsigned int currentLevel = 0;
};

} /* namespace glPortal */

#endif /* WORLD_HPP */
