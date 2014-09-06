#ifndef WORLD_HPP
#define WORLD_HPP

#include <string>
#include "Scene.hpp"

namespace glPortal {

const float GRAVITY = 0.01;
const float FRICTION = 0.01;

class Entity;
class Renderer;
class BoxCollider;
class Vector3f;

class World {
public:
  void create();
  void destroy();
  void update();
  void loadScene(std::string path);
  bool collidesWithWalls(BoxCollider collider);
  void shootPortal(int button);
  bool collides(Vector3f ro, Vector3f rd, Entity e, float* tNear, float* tFar);
  void render();

  static float gravity;
  static float friction;
private:
  Renderer *renderer;
  Scene *scene;
};

} /* namespace glPortal */

#endif /* WORLD_HPP */
