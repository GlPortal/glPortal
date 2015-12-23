#include "PhysicsHelper.hpp"
#include <cmath>
#include <bullet/BulletCollision/CollisionShapes/btCollisionShape.h>
#include <bullet/BulletCollision/CollisionShapes/btPolyhedralConvexShape.h>
#include <Portal.hpp>

namespace glPortal {

bool PhysicsHelper::pointInAABB(const btVector3 &p, const btVector3 &min, const btVector3 &max) {
  return p.x() >= min.x() && p.x() <= max.x() &&
         p.y() >= min.y() && p.y() <= max.y() &&
         p.z() >= min.z() && p.z() <= max.z();
}

bool PhysicsHelper::pointInVolume(const btVector3 &p, const btCollisionObject &o) {
  btVector3 min, max;
  const btCollisionShape &s = *o.getCollisionShape();
  const btTransform &t = o.getWorldTransform();
  s.getAabb(t, min, max);
  // So far the following technique only works on collision shapes inheriting
  // from btPolyhedralConvexShape. Check for those.
  if (pointInAABB(p, min, max) && s.isPolyhedral() && s.isConvex()) {
    // Stupid margins.
    const btPolyhedralConvexShape &cs = (const btPolyhedralConvexShape&)s;
    return cs.isInside(t.inverse()(p), cs.getMargin());
  }
  return false;
}

#if 0
PhysicsHelper::ContactResultHolder cb;
btDefaultMotionState ms;
ms.setWorldTransform(btTransform(btQuaternion(), p));
btSphereShape sh(0);
btRigidBody::btRigidBodyConstructionInfo ci(0, &ms, &sh, btVector3(0, 0, 0));
btRigidBody rb(ci);

physics.world->contactPairTest(OBJ, &rb, cb);
if (cb.collides) {
  return true;
}
#endif

} /* namespace glPortal */
