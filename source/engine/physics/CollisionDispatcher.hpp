#ifndef COLLISION_DISPATCHER_HPP
#define COLLISION_DISPATCHER_HPP

#include <bullet/btBulletDynamicsCommon.h>

namespace glPortal {

class CollisionDispatcher : public btCollisionDispatcher {
public:
  using btCollisionDispatcher::btCollisionDispatcher;

  //bool needsCollision(const btCollisionObject*, const btCollisionObject*) final override;
  //bool needsResponse(const btCollisionObject*, const btCollisionObject*) final override;
};

} /* namespace glPortal */

#endif /* COLLISION_DISPATCHER_HPP */
