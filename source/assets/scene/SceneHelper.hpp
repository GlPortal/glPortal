#ifndef SCENE_HELPER_HPP
#define SCENE_HELPER_HPP

#include <utility>
#include <assets/scene/Scene.hpp>
#include <engine/Entity.hpp>
#include <engine/component/Transform.hpp>
#include <engine/component/LightSource.hpp>

namespace glPortal {

class SceneHelper {
public:
  static EntityPair& getPortalPairFromScene(int n, Scene *scene);
};

} /* namespace glPortal */

#endif /* SCENE_HELPER_HPP */
