#ifndef PORTAL_SYSTEM_HPP
#define PORTAL_SYSTEM_HPP

#include <radix/system/System.hpp>

namespace glPortal {

class PortalSystem : public radix::System {
public:
  PortalSystem(radix::World&);
  ~PortalSystem();

  void update(float dtime);
};

} /* namespace glPortal */

#endif /* PORTAL_SYSTEM_HPP */
