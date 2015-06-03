#ifndef SCENE_HPP
#define SCENE_HPP

#include <vector>
#include <map>

#include <assets/gui/GUIButton.hpp>
#include <assets/material/Material.hpp>
#include "engine/Camera.hpp"
#include "engine/Light.hpp"
#include "engine/volumes/Volume.hpp"
#include "engine/trigger/Trigger.hpp"
#include "Player.hpp"
#include "Portal.hpp"

namespace glPortal {

class Scene {
public:
  Player player;
  Camera camera;
  std::map<int, Material> materials;
  std::vector<PhysicsEntity> walls;
  std::vector<Volume> volumes;
  std::vector<Trigger> triggers;
  std::vector<Light> lights;
  std::vector<VisualEntity> models;
  std::vector<GUIButton> buttons;
  Portal bluePortal;
  Portal orangePortal;
  VisualEntity start;
  VisualEntity end;
};

} /* namespace glPortal */

#endif /* SCENE_HPP */
