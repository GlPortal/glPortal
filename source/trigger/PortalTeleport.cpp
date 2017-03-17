#include <glPortal/trigger/PortalTeleport.hpp>
#include <radix/data/map/XmlHelper.hpp>
#include <radix/component/Trigger.hpp>
#include <radix/component/Player.hpp>

namespace glPortal {

PortalTeleport::PortalTeleport() {
  TYPE = "portalTeleport";
  loadFunction = [] (radix::Entity &trigger, tinyxml2::XMLElement *xmlElement) {
    std::string destination = radix::XmlHelper::extractStringMandatoryAttribute(xmlElement,
                                                                                "destination");
    PortalTeleport::setAction(trigger, destination);
  };
}

void PortalTeleport::setAction(radix::Entity &trigger, std::string &destination) {
  trigger.getComponent<radix::Trigger>().setActionOnUpdate([destination]
                                                             (radix::BaseGame &game) {
    radix::Transform &transform = game.getWorld()->getPlayer().getComponent<radix::Transform>();
    if (game.getWorld()->destinations.find(destination)
        != game.getWorld()->destinations.end()) {
      transform.setPosition(game.getWorld()->destinations.at(destination).position);
    }
  });
}

} /* namespace glPortal */