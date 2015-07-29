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

bool PortalHelper::goesThroughPortal(const Entity &entity, const BoxCollider &collider) {
  const Transform &t = entity.getComponent<Transform>();
  const Vector3f &rotation = t.rotation;
  const Vector3f &position = t.position;
  const Vector3f &scale = t.scale;

  if (rotation.x == 0) {
    if (rotation.y == rad(90)) {
      if (collider.position.x > position.x &&
         collider.position.x + collider.size.x/2 < position.x + scale.x &&
         collider.position.y - collider.size.y/2 > position.y - scale.y/2 &&
         collider.position.y + collider.size.y/2 < position.y + scale.y/2 &&
         collider.position.z - collider.size.z/2 > position.z - scale.z/2 &&
         collider.position.z + collider.size.z/2 < position.z + scale.z/2) {
        return true;
      }
    }
    if (rotation.y == rad(-90)) {
      if (collider.position.x < position.x &&
         collider.position.x - collider.size.x/2 > position.x - scale.x &&
         collider.position.y - collider.size.y/2 > position.y - scale.y/2 &&
         collider.position.y + collider.size.y/2 < position.y + scale.y/2 &&
         collider.position.z - collider.size.z/2 > position.z - scale.z/2 &&
         collider.position.z + collider.size.z/2 < position.z + scale.z/2) {
        return true;
      }
    }
    if (rotation.y == 0) {
      if (collider.position.x - collider.size.x/2 > position.x - scale.x/2 &&
         collider.position.x + collider.size.x/2 < position.x + scale.x/2 &&
         collider.position.y - collider.size.y/2 > position.y - scale.y/2 &&
         collider.position.y + collider.size.y/2 < position.y + scale.y/2 &&
         collider.position.z > position.z &&
         collider.position.z + collider.size.z/2 < position.z + scale.z) {
        return true;
      }
    }
    if (rotation.y == rad(180)) {
      if (collider.position.x - collider.size.x/2 > position.x - scale.x/2 &&
         collider.position.x + collider.size.x/2 < position.x + scale.x/2 &&
         collider.position.y - collider.size.y/2 > position.y - scale.y/2 &&
         collider.position.y + collider.size.y/2 < position.y + scale.y/2 &&
         collider.position.z < position.z &&
         collider.position.z - collider.size.z/2 > position.z - scale.z) {
        return true;
      }
    }
  } else {
    if (rotation.x == rad(-90)) {
      if (collider.position.x - collider.size.x/2 >= position.x - scale.x/2 &&
         collider.position.x + collider.size.x/2 <= position.x + scale.x/2 &&
         collider.position.y > position.y &&
         collider.position.y + collider.size.y/2 <= position.y + scale.y &&
         collider.position.z - collider.size.z/2 >= position.z - scale.z/2 &&
         collider.position.z + collider.size.z/2 <= position.z + scale.z/2) {
        return true;
      }
    }
    if (rotation.x == rad(90)) {
      if (collider.position.x - collider.size.x/2 >= position.x - scale.x/2 &&
         collider.position.x + collider.size.x/2 <= position.x + scale.x/2 &&
         collider.position.y < position.y &&
         collider.position.y - collider.size.y/2 >= position.y - scale.y &&
         collider.position.z - collider.size.z/2 >= position.z - scale.z/2 &&
         collider.position.z + collider.size.z/2 <= position.z + scale.z/2) {
        return true;
      }
    }
  }
  return false;
}

bool PortalHelper::isInPortal(const Entity &entity, const BoxCollider &collider) {
  const Transform &t = entity.getComponent<Transform>();
  const Vector3f &rotation = t.rotation;
  const Vector3f &position = t.position;
  const Vector3f &scale = t.scale;

  if (rotation.x == 0) {
    if (rotation.y == rad(90)) {
      if (collider.position.x + collider.size.x/2 > position.x &&
         collider.position.x + collider.size.x/2 < position.x + scale.x &&
         collider.position.y - collider.size.y/2 > position.y - scale.y/2 &&
         collider.position.y + collider.size.y/2 < position.y + scale.y/2 &&
         collider.position.z - collider.size.z/2 > position.z - scale.z/2 &&
         collider.position.z + collider.size.z/2 < position.z + scale.z/2) {
        return true;
      }
    }
    if (rotation.y == rad(-90)) {
      if (collider.position.x - collider.size.x/2  < position.x &&
         collider.position.x - collider.size.x/2 > position.x - scale.x &&
         collider.position.y - collider.size.y/2 > position.y - scale.y/2 &&
         collider.position.y + collider.size.y/2 < position.y + scale.y/2 &&
         collider.position.z - collider.size.z/2 > position.z - scale.z/2 &&
         collider.position.z + collider.size.z/2 < position.z + scale.z/2) {
        return true;
      }
    }
    if (rotation.y == 0) {
      if (collider.position.x - collider.size.x/2 > position.x - scale.x/2 &&
         collider.position.x + collider.size.x/2 < position.x + scale.x/2 &&
         collider.position.y - collider.size.y/2 > position.y - scale.y/2 &&
         collider.position.y + collider.size.y/2 < position.y + scale.y/2 &&
         collider.position.z + collider.size.z/2 > position.z &&
         collider.position.z + collider.size.z/2 < position.z + scale.z) {
        return true;
      }
    }
    if (rotation.y == rad(180)) {
      if (collider.position.x - collider.size.x/2 > position.x - scale.x/2 &&
         collider.position.x + collider.size.x/2 < position.x + scale.x/2 &&
         collider.position.y - collider.size.y/2 > position.y - scale.y/2 &&
         collider.position.y + collider.size.y/2 < position.y + scale.y/2 &&
         collider.position.z - collider.size.z/2 < position.z &&
         collider.position.z - collider.size.z/2 > position.z - scale.z) {
        return true;
      }
    }
  } else {
    if (rotation.x == rad(-90)) {
      if (collider.position.x - collider.size.x/2 > position.x - scale.x/2 &&
         collider.position.x + collider.size.x/2 < position.x + scale.x/2 &&
         collider.position.y + collider.size.y/2 > position.y &&
         collider.position.y + collider.size.y/2 < position.y + scale.y &&
         collider.position.z - collider.size.z/2 > position.z - scale.z/2 &&
         collider.position.z + collider.size.z/2 < position.z + scale.z/2) {
        return true;
      }
    }
    if (rotation.x == rad(90)) {
      if (collider.position.x - collider.size.x/2 > position.x - scale.x/2 &&
         collider.position.x + collider.size.x/2 < position.x + scale.x/2 &&
         collider.position.y - collider.size.y/2 < position.y &&
         collider.position.y - collider.size.y/2 > position.y - scale.y &&
         collider.position.z - collider.size.z/2 > position.z - scale.z/2 &&
         collider.position.z + collider.size.z/2 < position.z + scale.z/2) {
        return true;
      }
    }
  }
  return false;
} 
} /* namespace glPortal */
