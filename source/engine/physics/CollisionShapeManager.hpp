#ifndef COLLISION_SHAPE_MANAGER_HPP
#define COLLISION_SHAPE_MANAGER_HPP

#include <engine/core/math/Vector3f.hpp>
#include <engine/Entity.hpp>

namespace glPortal {

class CollisionShapeManager {
public:
  BoxCollider();
  BoxCollider(const Vector3f &position, const Vector3f &size);
  bool collidesWith(const BoxCollider &collider) const;
  static BoxCollider generateCage(const Entity &entity);

  Vector3f position;
  Vector3f size;
};

} /* namespace glPortal */

#endif /* COLLISION_SHAPE_MANAGER_HPP */
