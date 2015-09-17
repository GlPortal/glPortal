#include "BoxCollider.hpp"

#include <engine/core/math/Matrix4f.hpp>
#include <engine/Entity.hpp>
#include <engine/component/Transform.hpp>
#include <engine/component/MeshDrawable.hpp>

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
  const Transform &t = entity.getComponent<Transform>();
  const Vector3f &position = t.position;
  const Vector3f &scale = t.scale;

  if (entity.hasComponent<MeshDrawable>()) {
    Mesh &mesh = entity.getComponent<MeshDrawable>().mesh;
    Matrix4f modelMatrix;
    modelMatrix.rotate(t.orientation);
    modelMatrix.scale(scale);

    Vector3f size;
    for (unsigned int i = 0; i < mesh.vertices.size(); i++) {
      Vector3f v = modelMatrix.transform(mesh.vertices[i]);

      size.x = (fabs(v.x) > size.x) ? fabs(v.x) : size.x;
      size.y = (fabs(v.y) > size.y) ? fabs(v.y) : size.y;
      size.z = (fabs(v.z) > size.z) ? fabs(v.z) : size.z;
    }
    return BoxCollider(position, size * 2);
  }
  return BoxCollider(position, scale * 2);
}

} /* namespace glPortal */

