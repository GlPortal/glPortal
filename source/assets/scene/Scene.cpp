#include "Scene.hpp"

namespace glPortal {

Scene::Scene() {
  this->screen = std::unique_ptr<Screen>(new Screen("GlPortal", "Hit enter to start."));
}
} /* namespace glPortal */

