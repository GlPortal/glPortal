#ifndef WORLD_HELPER_HPP
#define WORLD_HELPER_HPP

#include <utility>
#include <assets/scene/Scene.hpp>
#include <engine/Entity.hpp>
#include <engine/component/Transform.hpp>
#include <engine/component/LightSource.hpp>

namespace glPortal {

class WorldHelper {
public:
  static bool isPlayerPortalingX(BoxCollider &box, Entity *player, Scene *scene);
  static bool isPlayerPortalingY(BoxCollider &box, Entity *player, Scene *scene);
  static bool isPlayerPortalingZ(BoxCollider &box, Entity *player, Scene *scene);
};

} /* namespace glPortal */

#endif /* WORLD_HELPER_HPP */
