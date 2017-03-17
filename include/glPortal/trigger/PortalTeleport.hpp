#ifndef GLPORTAL_CUSTOM_TRIGGERS_HPP
#define GLPORTAL_CUSTOM_TRIGGERS_HPP

#include <radix/data/map/CustomTrigger.hpp>

namespace glPortal {

class PortalTeleport : public radix::CustomTrigger {
public:
  PortalTeleport();
  static void setAction(radix::Entity &trigger, std::string &destination);
};

} /* namespace glPortal */

#endif /* GLPORTAL_CUSTOMTRIGGERS_HPP */
