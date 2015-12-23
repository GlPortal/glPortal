#ifndef UNCOLLIDER_HPP
#define UNCOLLIDER_HPP

#include <bullet/BulletCollision/BroadphaseCollision/btOverlappingPairCache.h>
#include <assets/scene/Scene.hpp>

namespace glPortal {

class Uncollider {
private:
  Scene &scene;

public:
  static std::vector<Entity*> portals;
  static bool isPointInUncollideVolume(const btVector3 &p);

  Uncollider(Scene&);
  void beforePhysicsStep();
  static void nearCallback(btBroadphasePair&, btCollisionDispatcher&, const btDispatcherInfo&);
};

} /* namespace glPortal */

#endif /* UNCOLLIDER_HPP */
