#include "BoxCollider.hpp"

#include "engine/Entity.hpp"
#include "engine/core/math/Matrix4f.hpp"

#include <cmath>
#include <cstdio>

namespace glPortal {
  
BoxCollider::BoxCollider() {
}

BoxCollider::BoxCollider(const Vector3f &position, const Vector3f &size) {
  this->position.set(position);
  this->size.set(size);
}

bool BoxCollider::collidesWith(const BoxCollider &collider) const {
  if(position.x - size.x/2 < collider.position.x + collider.size.x/2 &&
      position.x + size.x/2 > collider.position.x - collider.size.x/2 &&
      position.y - size.y/2 < collider.position.y + collider.size.y/2 &&
      position.y + size.y/2 > collider.position.y - collider.size.y/2 &&
      position.z - size.z/2 < collider.position.z + collider.size.z/2 &&
      position.z + size.z/2 > collider.position.z - collider.size.z/2) {
    return true;
  }
  return false;
}

BoxCollider BoxCollider::generateCage(const Entity &entity) {
  Matrix4f modelMatrix;
  //modelMatrix.translate(entity.position);
  modelMatrix.rotate(entity.rotation);
  modelMatrix.scale(entity.scale);

  Vector3f position;
  position.set(entity.position);
  Vector3f size;
  for (unsigned int i = 0; i < entity.mesh.vertices.size(); i++) {
    Vector3f v = modelMatrix.transform(entity.mesh.vertices[i]);

    size.x = (fabs(v.x) > size.x) ? fabs(v.x) : size.x;
    size.y = (fabs(v.y) > size.y) ? fabs(v.y) : size.y;
    size.z = (fabs(v.z) > size.z) ? fabs(v.z) : size.z;
  }

  return BoxCollider(position, size * 2);
}

} /* namespace glPortal */

