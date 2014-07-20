#ifndef WORLD_HPP
#define WORLD_HPP

namespace glPortal {

class Entity;
class Renderer;
class Scene;
class Vector3f;

class World {
public:
  void create();
  void update();
  void loadScene(Scene scene);
  void usePortal(Entity srcPortal, Entity destPortal);
  void shootPortal(int button);
  bool collides(Vector3f ro, Vector3f rd, Entity e, float* tNear, float* tFar);

  void render();

private:
  Renderer *renderer;
  Scene* scene;
};

} /* namespace glPortal */

#endif /* WORLD_HPP */
