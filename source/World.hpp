#ifndef WORLD_HPP
#define WORLD_HPP

#include "engine/Renderer.hpp"

namespace glPortal {

class World {
public:
  void create();
  void update();
  void render();

private:
  Renderer *renderer;
  Scene* scene;
};

} /* namespace glPortal */

#endif /* WORLD_HPP */
