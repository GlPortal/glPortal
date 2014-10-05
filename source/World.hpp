#ifndef WORLD_HPP
#define WORLD_HPP

#include <string>
#include "Scene.hpp"

namespace glPortal {

class Entity;
class Renderer;
  //class Scene;
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
  void collidePlayer();
  bool collidesWithWallOutsidePortal(BoxCollider collider);
  void render();

private:
  Renderer *renderer;
  Scene *scene;
};

} /* namespace glPortal */

#endif /* WORLD_HPP */
