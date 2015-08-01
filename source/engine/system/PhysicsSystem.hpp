#ifndef PHYSICS_SYSTEM_HPP
#define PHYSICS_SYSTEM_HPP
#include <assets/scene/Scene.hpp>

namespace glPortal {

class PhysicsSystem {
private:
  Scene *scene;

public:
  void setScene(Scene *scene);
  void update(float dtime);
};

} /* namespace glPortal */

#endif /* PHYSICS_SYSTEM_HPP */
