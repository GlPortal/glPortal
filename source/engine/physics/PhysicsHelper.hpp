#ifndef PHYSICS_HELPER_HPP
#define PHYSICS_HELPER_HPP

#include <bullet/LinearMath/btVector3.h>
#include <bullet/BulletCollision/Gimpact/btBoxCollision.h>
#include <bullet/BulletCollision/CollisionDispatch/btCollisionObject.h>
#include <bullet/BulletCollision/CollisionDispatch/btCollisionWorld.h>

namespace glPortal {

class PhysicsHelper {
private:
  PhysicsHelper();

public:
  class ContactResultHolder : public btCollisionWorld::ContactResultCallback {
  public:
    bool collides = false;
    btScalar addSingleResult(btManifoldPoint&, const btCollisionObjectWrapper*, int, int,
      const btCollisionObjectWrapper*, int, int) {
      collides = true;
      return 1;
    }
  };

  static bool pointInAABB(const btVector3 &p, const btVector3 &min, const btVector3 &max);
  static inline bool pointInAABB(const btVector3 &p, const btAABB &aabb) {
    return pointInAABB(p, aabb.m_min, aabb.m_max);
  }

  static bool pointInVolume(const btVector3&, const btCollisionObject&);
  static inline bool pointInVolume(const btVector3 &v, const btCollisionObject *o) {
    return pointInVolume(v, *o);
  }
};

} /* namespace glPortal */

#endif /* PHYSICS_HELPER_HPP */
