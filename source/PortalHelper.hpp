#ifndef PORTAL_HELPER_HPP
#define PORTAL_HELPER_HPP

#include <utility>
#include <assets/scene/Scene.hpp>

#include <engine/component/Component.hpp>
#include <engine/core/math/Vector3f.hpp>
#include <engine/BoxCollider.hpp>
#include <cmath>
#include <engine/Entity.hpp>

namespace glPortal {

class PortalHelper {
public:
  static float* getDistancesForPoint(const Vector3f &point, const BoxCollider &wall);
  static float getDistanceFromCoordinates(const float &contactCoordinate, const float &outerBoundCoordinate);
  static bool goesThroughPortal(const Entity &entity, const BoxCollider &collider);
  static bool isInPortal(const Entity &entity, const BoxCollider &collider);
};

} /* namespace glPortal */

#endif /* PORTAL_HELPER_HPP */
