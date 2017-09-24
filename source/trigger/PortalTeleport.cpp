#include <glPortal/trigger/PortalTeleport.hpp>
#include <radix/data/map/XmlHelper.hpp>
#include <radix/entities/Trigger.hpp>
#include <radix/entities/Player.hpp>
#include <radix/BaseGame.hpp>

namespace glPortal {

PortalTeleport::PortalTeleport() {
  TYPE = "portalTeleport";
  loadFunction = [] (radix::Entity &trigger, tinyxml2::XMLElement *xmlElement) {
    std::string destination = radix::XmlHelper::extractStringMandatoryAttribute(
        xmlElement, "destination");
    PortalTeleport::setAction(trigger, destination);
  };
}

void PortalTeleport::setAction(radix::Entity &trigger, std::string &destination) {
  dynamic_cast<radix::entities::Trigger&>(trigger).setActionOnUpdate([&trigger, destination]
                                                             (radix::entities::Trigger &trigger) {
    if (trigger.world.destinations.find(destination)
        != trigger.world.destinations.end()) {
      trigger.setPosition(trigger.world.destinations.at(destination).position);
    }
  });
}

} /* namespace glPortal */
