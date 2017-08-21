#include <glPortal/trigger/PortalTeleport.hpp>
#include <radix/data/map/XmlHelper.hpp>
#include <radix/entities/Trigger.hpp>
#include <radix/entities/Player.hpp>

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
                                                             (radix::BaseGame &game) {
    if (game.getWorld()->destinations.find(destination)
        != game.getWorld()->destinations.end()) {
      trigger.setPosition(game.getWorld()->destinations.at(destination).position);
    }
  });
}

} /* namespace glPortal */
