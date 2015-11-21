#ifndef SCENE_HPP
#define SCENE_HPP

#include <vector>
#include <map>
#include <list>

#include <assets/gui/GUIButton.hpp>
#include <assets/material/Material.hpp>
#include <engine/Camera.hpp>
#include "Portal.hpp"
#include "Screen.hpp"

namespace glPortal {

typedef std::pair<Entity*, Entity*> EntityPair;

class Scene {
public:
  Scene();
  Entity player;
  Entity terminal;
  Camera camera;
  std::map<int, Material> materials;
  std::list<Entity> entities;
  std::vector<EntityPair> portalPairs;
  std::vector<GUIButton> buttons;
  std::unique_ptr<Screen> screen;
  Entity start;
  Entity end;
};

} /* namespace glPortal */

#endif /* SCENE_HPP */
