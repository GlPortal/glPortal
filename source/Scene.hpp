#ifndef SCENE_HPP
#define SCENE_HPP

#include "engine/Camera.hpp"
#include "engine/Entity.hpp"
#include "engine/Light.hpp"
#include "Player.hpp"
#include <vector>

namespace glPortal {

class Scene {
public:
  Player player;
  Camera camera;
  std::vector<Entity> walls;
  std::vector<Light> lights;
};

} /* namespace glPortal */

#endif /* SCENE_HPP */
