#include <glPortal/trigger/PortalTeleport.hpp>

#include <radix/data/map/XmlHelper.hpp>
#include <radix/entities/Trigger.hpp>
#include <radix/entities/Player.hpp>
#include <radix/BaseGame.hpp>
#include <radix/World.hpp>

namespace glPortal {

PortalTeleport::PortalTeleport() {
  TYPE = "portalTeleport";
  loadFunction = [] (radix::Entity &trigger, tinyxml2::XMLElement *xmlElement) {
    std::string destination = radix::XmlHelper::extractStringMandatoryAttribute(
        xmlElement, "destination");
    PortalTeleport::setAction(trigger, destination);
  };
}

void PortalTeleport::setAction(radix::Entity &trigger, const std::string &destination) {
  dynamic_cast<radix::entities::Trigger&>(trigger).setActionOnMove([&trigger, destination]
                                                             (radix::entities::Trigger &trigger) {
    radix::entities::Player &player = trigger.world.getPlayer();
    if (trigger.world.destinations.find(destination) != trigger.world.destinations.end()) {
      const radix::Quaternion &destinationOrientation = trigger.world.destinations.at(destination).orientation;
      radix::Quaternion newPlayerOrientation = destinationOrientation;

      player.setPosition(trigger.world.destinations.at(destination).position);
      player.setOrientation(newPlayerOrientation);
      player.setHeadOrientation(newPlayerOrientation);
    }
  });
}

} /* namespace glPortal */
