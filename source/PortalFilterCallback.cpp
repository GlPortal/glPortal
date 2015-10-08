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
static bool testAABB(const btVector3 &min, const btVector3 &max, const btVector3 &p) {
  bool xIn = p.x() > min.x() && p.x() < max.x();
  bool yIn = p.y() > min.y() && p.y() < max.y();
  bool zIn = p.z() > min.z() && p.z() < max.z();
  return xIn and yIn and zIn;
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

void PortalFilterCallback::nearCallback(btBroadphasePair &collisionPair,
  btCollisionDispatcher &dispatcher, const btDispatcherInfo &dispatchInfo) {

  btCollisionObject *colObj0 = (btCollisionObject*)collisionPair.m_pProxy0->m_clientObject;
  btCollisionObject *colObj1 = (btCollisionObject*)collisionPair.m_pProxy1->m_clientObject;
  
  if (dispatcher.needsCollision(colObj0, colObj1)) {
    btCollisionObjectWrapper obj0Wrap(0, colObj0->getCollisionShape(),
      colObj0, colObj0->getWorldTransform(), -1, -1);
    btCollisionObjectWrapper obj1Wrap(0, colObj1->getCollisionShape(),
      colObj1, colObj1->getWorldTransform(), -1, -1);

    if (!collisionPair.m_algorithm) {
      collisionPair.m_algorithm = dispatcher.findAlgorithm(&obj0Wrap, &obj1Wrap);
    }

    if (collisionPair.m_algorithm) {
      btManifoldResult contactPointResult(&obj0Wrap, &obj1Wrap);
      if (dispatchInfo.m_dispatchFunc == btDispatcherInfo::DISPATCH_DISCRETE) {
        collisionPair.m_algorithm->processCollision(&obj0Wrap, &obj1Wrap, dispatchInfo, &contactPointResult);
        for (int i = 0; i < contactPointResult.getPersistentManifold()->getNumContacts(); ++i) {
          const btManifoldPoint &pt = contactPointResult.getPersistentManifold()->getContactPoint(i);
          const btVector3 &cp = pt.getPositionWorldOnA();
          if (testAABB(clipMin, clipMax, cp)) {
            contactPointResult.getPersistentManifold()->removeContactPoint(i--);
          }
        }
      } else {
        btScalar toi = collisionPair.m_algorithm->calculateTimeOfImpact(colObj0, colObj1, dispatchInfo, &contactPointResult);
        if (dispatchInfo.m_timeOfImpact > toi)
          dispatchInfo.m_timeOfImpact = toi;
      }
    }
  }
}
  
} /* namespace glPortal */
