#include "PortalSystem.hpp"
#include <vector>
#include <engine/component/Player.hpp>
#include <engine/component/Transform.hpp>
#include <Portal.hpp>

namespace glPortal {

void PortalSystem::setScene(Scene *scene) {
  this->scene = scene;
}

PortalSystem::PortalSystem() :
  scene(nullptr) {
}

PortalSystem::~PortalSystem() {
}

void PortalSystem::update(float dtime) {
  for (Entity &e : scene->entities) {
    if (e.hasComponent<Player>()) {
      Transform &t = e.getComponent<Transform>();
      const Vector3f &pos = t.getPosition();
      // Player &p = e.getComponent<Player>();
      for (const EntityPair &e : scene->portalPairs) {
        // Behind portal A
        Vector3f normalA = e.first->getComponent<Portal>().getDirection();
        Vector3f AP = pos - e.first->getComponent<Transform>().getPosition();
        if (dot(AP, normalA) < 0) {
          t.setPosition(e.second->getComponent<Transform>().getPosition() + AP);
        }
        // Behind portal B
        Vector3f normalB = e.second->getComponent<Portal>().getDirection();
        Vector3f BP = pos - e.second->getComponent<Transform>().getPosition();
        if (dot(BP, normalB) < 0) {
          t.setPosition(e.first->getComponent<Transform>().getPosition() + BP);
        }
      }
    }
  }
}

} /* namespace glPortal */
