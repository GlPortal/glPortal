#include "PortalSystem.hpp"
#include <vector>
#include <engine/component/Player.hpp>
#include <engine/component/Transform.hpp>
#include <engine/component/Player.hpp>
#include <Portal.hpp>
#include "../physics/PhysicsHelper.hpp"

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
  (void) dtime;
  for (Entity &e : scene->entities) {
    if (e.hasComponent<Player>()) {
      Transform &t = e.getComponent<Transform>();
      const Vector3f &pos = t.getPosition();
      Player &plr = e.getComponent<Player>();
      for (const EntityPair &pp : scene->portalPairs) {
        btVector3 min, max;
        Portal &pA = pp.first->getComponent<Portal>(),
               &pB = pp.second->getComponent<Portal>();

        // No uncollider? No portal link.
        if (not pA.uncollider or not pB.uncollider) {
          continue;
        }

        pA.uncolliderShape->getAabb(
          pA.uncollider->getWorldTransform(),
          min, max);
        // Is player within portal A uncollider (AABB then real check) ?
        if (PhysicsHelper::pointInAABB(pos, min, max) and
            PhysicsHelper::pointInVolume(pos, *pA.uncollider)) {
          // Behind portal A
          const Transform
            &pAT = pp.first->getComponent<Transform>(),
            &pBT = pp.second->getComponent<Transform>();
          Vector3f normalA = pA.getDirection();
          Vector3f AP = pos - pAT.getPosition();
          if (dot(AP, normalA) < 0) {
            Matrix4f rotate180; rotate180.rotate(rad(180), 0, 1, 0);
            Matrix4f view = inverse(plr.getBaseHeadOrientation().toMatrix());
            view.translate(-t.getPosition());
            Matrix4f iv = inverse( view *
              Matrix4f(pAT.getPosition(), pAT.getOrientation()) *
              rotate180 *
              inverse(Matrix4f(pBT.getPosition(), pBT.getOrientation())) );
            t.setPosition(iv.getPosition());
            plr.headAngle = iv.getRotation().toAero();
          }
        }
        pB.uncolliderShape->getAabb(
          pB.uncollider->getWorldTransform(),
          min, max);
        // Is player within portal B uncollider ?
        if (PhysicsHelper::pointInAABB(pos, min, max) and
            PhysicsHelper::pointInVolume(pos, *pB.uncollider)) {
          // Behind portal B
          const Transform
            &pAT = pp.first->getComponent<Transform>(),
            &pBT = pp.second->getComponent<Transform>();
          Vector3f normalB = pB.getDirection();
          Vector3f BP = pos - pBT.getPosition();
          if (dot(BP, normalB) < 0) {
            Matrix4f rotate180; rotate180.rotate(rad(180), 0, 1, 0);
            Matrix4f view = inverse(plr.getBaseHeadOrientation().toMatrix());
            view.translate(-t.getPosition());
            Matrix4f iv = inverse( view *
              Matrix4f(pBT.getPosition(), pBT.getOrientation()) *
              rotate180 *
              inverse(Matrix4f(pAT.getPosition(), pAT.getOrientation())) );
            t.setPosition(Vector3f(iv[12], iv[13], iv[14]));
            plr.headAngle = iv.getRotation().toAero();
          }
        }
      }
    }
  }
}

} /* namespace glPortal */
