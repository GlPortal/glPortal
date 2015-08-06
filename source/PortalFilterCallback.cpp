#include "PortalFilterCallback.hpp"
#include <cmath>

namespace glPortal {

PortalFilterCallback::PortalFilterCallback(Scene &scene) : 
  scene(scene) {
}


// TODO: Move to BoxCollider / AABB
static bool testAABB(const btVector3 &amin, const btVector3 &amax,
  const btVector3 &bmin, const btVector3 &bmax) {
  bool xOverlap = !(bmin.x() > amax.x() || bmax.x() < amin.x());
  bool yOverlap = !(bmin.y() > amax.y() || bmax.y() < amin.y());
  bool zOverlap = !(bmin.z() > amax.z() || bmax.z() < amin.z());
  return xOverlap and yOverlap and zOverlap;
}

static const btVector3 clipMin(2, 0, -1), clipMax(3, 2, 1);

void PortalFilterCallback::beforePhysicsStep() {
  scene.physics.world->getDebugDrawer()->drawBox(clipMin, clipMax, btVector3(1, .5, 0));
}

bool PortalFilterCallback::needBroadphaseCollision(btBroadphaseProxy *proxy1,
  btBroadphaseProxy* proxy2) const {
  bool prox1Static = false, // proxy1->isNonMoving(),
       prox2Static = true;//proxy2->isNonMoving();
  if (prox1Static xor prox2Static) {
    btBroadphaseProxy *proxMoving = prox1Static ? proxy1 : proxy2;
    return not testAABB(proxMoving->m_aabbMin, proxMoving->m_aabbMax, clipMin, clipMax);
  }
}
  
} /* namespace glPortal */
