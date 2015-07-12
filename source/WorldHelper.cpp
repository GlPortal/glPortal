#include "WorldHelper.hpp"

namespace glPortal {
bool WorldHelper::isPlayerPortalingX(BoxCollider &box, Entity *player, Scene *scene) {
  bool portaling = false;
  for (const EntityPair &p : scene->portalPairs) {
    const Portal &portal1 = p.first->getComponent<Portal>(),
      &portal2 = p.second->getComponent<Portal>();
    if (portal1.open and portal2.open) {
      const Transform &p1Tform = p.first->getComponent<Transform>(),
        &p2Tform = p.second->getComponent<Transform>();
      if (portal1.inPortal(box)) {
        if (p1Tform.rotation.x == 0 and (p1Tform.rotation.y == rad(-90) || p1Tform.rotation.y == rad(90))) {
          portaling = true;
        }
      }
      if (portal2.inPortal(box)) {
        if (p2Tform.rotation.x == 0 and (p2Tform.rotation.y == rad(-90) || p2Tform.rotation.y == rad(90))) {
          portaling = true;
        }
      }
    }
  }
  return portaling;
}
  
bool WorldHelper::isPlayerPortalingY(BoxCollider &box, Entity *player, Scene *scene) {
  bool portaling = false;
  for (const EntityPair &p : scene->portalPairs) {
    const Portal &portal1 = p.first->getComponent<Portal>(),
      &portal2 = p.second->getComponent<Portal>();
    if (portal1.open and portal2.open) {
      const Transform &p1Tform = p.first->getComponent<Transform>(),
        &p2Tform = p.second->getComponent<Transform>();
      if (portal1.inPortal(box)) {
        if (p1Tform.rotation.x == rad(-90) || p1Tform.rotation.x == rad(90)) {
          portaling = true;
        }
      }
      if (portal2.inPortal(box)) {
        if (p2Tform.rotation.x == rad(-90) || p2Tform.rotation.x == rad(90)) {
          portaling = true;
        }
      }
    }
  }
  return portaling;
}
  
bool WorldHelper::isPlayerPortalingZ(BoxCollider &box, Entity *player, Scene *scene) {
  bool portaling = false;
  for (const EntityPair &p : scene->portalPairs) {
    const Portal &portal1 = p.first->getComponent<Portal>(),
      &portal2 = p.second->getComponent<Portal>();
    if (portal1.open and portal2.open) {
      const Transform &p1Tform = p.first->getComponent<Transform>(),
        &p2Tform = p.second->getComponent<Transform>();
      if (portal1.inPortal(box)) {
        if (p1Tform.rotation.x == 0 and (p1Tform.rotation.y == 0 || p1Tform.rotation.y == rad(180))) {
          portaling = true;
        }
      }
      if (portal2.inPortal(box)) {
        if (p2Tform.rotation.x == 0 and (p2Tform.rotation.y == 0 || p2Tform.rotation.y == rad(180))) {
          portaling = true;
        }
      }
    }
  }
  return portaling;
}
} /* namespace glPortal */
  
