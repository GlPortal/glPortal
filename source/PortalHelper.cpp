#include "PortalHelper.hpp"

#include <engine/component/Transform.hpp>

namespace glPortal {
float* PortalHelper::getDistancesForPoint(const Vector3f &point, const BoxCollider &wall) {
  Vector3f position = wall.position;
  Vector3f size = wall.size;
  float x = point.x;
  float y = point.y;
  float z = point.z;
  float *distances = new float[6];
  distances[0] = getDistanceFromCoordinates(x, position.x - size.x / 2);
  distances[1] = getDistanceFromCoordinates(x, position.x + size.x / 2);
  distances[2] = getDistanceFromCoordinates(z, position.z - size.z / 2);
  distances[3] = getDistanceFromCoordinates(z, position.z + size.z / 2);
  distances[4] = getDistanceFromCoordinates(y, position.y - size.y / 2);
  distances[5] = getDistanceFromCoordinates(y, position.y + size.y / 2);

  return distances;
}
float PortalHelper::getDistanceFromCoordinates(const float &contactCoordinate, const float &outerBoundCoordinate) {
  return (float) fabs(contactCoordinate - (outerBoundCoordinate));
}

bool PortalHelper::isInPortal(const Entity &entity, const BoxCollider &collider) {
  // TODO reimplment with Bullet
  return false;
}

} /* namespace glPortal */
