#ifndef PHYSICS_SYSTEM_HPP
#define PHYSICS_SYSTEM_HPP
#include <assets/scene/Scene.hpp>

namespace glPortal {

class PortalFilterCallback;

class PhysicsSystem {
private:
  Scene *scene;
  PortalFilterCallback *filterCallback;

public:
  PhysicsSystem();
  ~PhysicsSystem();
  void setScene(Scene *scene);
  void update(float dtime);
};

} /* namespace glPortal */

#endif /* PHYSICS_SYSTEM_HPP */
