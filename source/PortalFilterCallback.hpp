#ifndef PORTAL_FILTER_CALLBACK_HPP
#define PORTAL_FILTER_CALLBACK_HPP

#include <bullet/BulletCollision/BroadphaseCollision/btOverlappingPairCache.h>
#include <assets/scene/Scene.hpp>

namespace glPortal {

class PortalFilterCallback : public btOverlapFilterCallback {
private:
  Scene &scene;

public:
  PortalFilterCallback(Scene&);
  void beforePhysicsStep();
  bool needBroadphaseCollision(btBroadphaseProxy*, btBroadphaseProxy*) const;
};

} /* namespace glPortal */

#endif /* PORTAL_FILTER_CALLBACK_HPP */
