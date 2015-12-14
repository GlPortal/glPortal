#include "Scene.hpp"

namespace glPortal {

Scene::Scene() {
  this->screen   = std::make_unique<Screen>("GlPortal", "Hit enter to start.");
  this->terminal = std::make_unique<Terminal>();
}
} /* namespace glPortal */
