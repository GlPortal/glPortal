#ifndef BOXCOLLIDER_HPP
#define BOXCOLLIDER_HPP

#include <engine/core/math/Vector3f.hpp>

namespace glPortal {

class Entity;

class BoxCollider {
public:
  BoxCollider();
  BoxCollider(const Vector3f &position, const Vector3f &size);
  bool collidesWith(const BoxCollider &collider) const;
  static BoxCollider *generateCage(const Entity &entity);

  Vector3f position;
  Vector3f size;
};

} /* namespace glPortal */

#endif /* BOXCOLLIDER_HPP */
