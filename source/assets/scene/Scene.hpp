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
#include "Terminal.hpp"

namespace glPortal {

typedef std::pair<Entity*, Entity*> EntityPair;

class Scene {
public:
  Scene();
  Entity player;
  Camera camera;
  std::map<int, Material> materials;
  std::list<Entity> entities;
  std::vector<EntityPair> portalPairs;
  std::vector<GUIButton> buttons;
  std::unique_ptr<Screen> screen;
  std::unique_ptr<Terminal> terminal;
  Entity start;
  Entity end;
};

} /* namespace glPortal */

#endif /* SCENE_HPP */
