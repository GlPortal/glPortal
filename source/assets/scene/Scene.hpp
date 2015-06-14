#ifndef SCENE_HPP
#define SCENE_HPP

#include <vector>
#include <map>
#include <list>

#include <assets/gui/GUIButton.hpp>
#include <assets/material/Material.hpp>
#include <engine/Camera.hpp>
#include "Portal.hpp"

namespace glPortal {

typedef std::pair<Entity*, Entity*> EntityPair;

class Scene {
public:
  Entity player;
  Camera camera;
  std::map<int, Material> materials;
  std::list<Entity> entities;
  std::vector<EntityPair> portalPairs;
  std::vector<GUIButton> buttons;
  // FIXME: remove us!
  Entity start;
  Entity end;
};

} /* namespace glPortal */

#endif /* SCENE_HPP */
