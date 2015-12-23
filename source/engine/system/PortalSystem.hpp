#ifndef PORTAL_SYSTEM_HPP
#define PORTAL_SYSTEM_HPP
#include <assets/scene/Scene.hpp>

namespace glPortal {

class PortalSystem {
private:
  Scene *scene;

public:
  PortalSystem();
  ~PortalSystem();
  void setScene(Scene *scene);
  void update(float dtime);
};

} /* namespace glPortal */

#endif /* PORTAL_SYSTEM_HPP */
