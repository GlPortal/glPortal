#ifndef PORTAL_FILTER_CALLBACK_HPP
#define PORTAL_FILTER_CALLBACK_HPP

#include <bullet/BulletCollision/BroadphaseCollision/btOverlappingPairCache.h>
#include <assets/scene/Scene.hpp>

namespace glPortal {

class PortalFilterCallback {
private:
  Scene &scene;

public:
  static std::vector<Entity*> portals;

  PortalFilterCallback(Scene&);
  void beforePhysicsStep();
  static void nearCallback(btBroadphasePair&, btCollisionDispatcher&, const btDispatcherInfo&);
};

} /* namespace glPortal */

#endif /* PORTAL_FILTER_CALLBACK_HPP */
