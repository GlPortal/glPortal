#include <glPortal/WorldHelper.hpp>
#include <glPortal/Portal.hpp>
#include <glPortal/trigger/PortalTeleport.hpp>

#include <bullet/BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h>

#include <radix/data/texture/TextureLoader.hpp>
#include <radix/entities/traits/MeshDrawableTrait.hpp>
#include <radix/simulation/Physics.hpp>
#include <radix/entities/traits/LightSourceTrait.hpp>
#include <radix/entities/Trigger.hpp>
#include <radix/util/BulletUserPtrInfo.hpp>

using namespace radix;

namespace glPortal {

void WorldHelper::shootPortal(int button, World &world) {
  Vector3f cameraDir = Math::toDirection(world.camera->getOrientation());
  btVector3 btFrom = world.camera->getPosition();
  btVector3 btTo = btFrom + cameraDir*10000;
  btCollisionWorld::ClosestRayResultCallback res(btFrom, btTo);

  simulation::Physics &phys = world.simulations.findFirstOfType<simulation::Physics>();
  phys.getPhysicsWorld().rayTest(btFrom, btTo, res);

  if (res.hasHit()) {
    const Entity *pEnt = util::getBtPtrInfo(res.m_collisionObject).entity;
    // All RigidBodies should have their pointer set, but check anyway
    if (pEnt) {
      const Entity &ent = *pEnt;
      // TODO: material in separate Component, + 1 mat per face
      const auto *mdt = dynamic_cast<const entities::MeshDrawableTrait*>(&ent);
      if (mdt and
          mdt->material.portalable) {
        EntityPair &pPair = getPortalPair(0, world);
        Vector3f ipos(res.m_hitPointWorld);
        Entity &pEnt = (button == 1) ? *pPair.first : *pPair.second;
        Portal &portal = dynamic_cast<Portal&>(pEnt);
        portal.openSince = world.getTime();
        portal.maskTex.diffuse = TextureLoader::getTexture("portalmask.png");
        portal.placeOnWall(world.camera->getPosition(), ipos, res.m_hitNormalWorld);
        auto &pLight = static_cast<entities::LightSourceTrait&>(portal);
        
        if (portal.trigger == nullptr) {
          portal.trigger = &world.entityManager.create<radix::entities::Trigger>();
        }
        portal.trigger->setScale(portal.getScale());
        Vector3f modPos = ipos - ((portal.getScale() / 1.15) * portal.getDirection());
        portal.trigger->setPosition(modPos);
        Destination destination;
        destination.position = ipos + portal.getDirection();
        destination.rotation = Vector3f{};
        destination.orientation = portal.getOrientation();
        if (button == 1) {
          world.destinations["portal1"] = destination;
          PortalTeleport::setAction(*portal.trigger, "portal2");
          portal.overlayTex.diffuse = TextureLoader::getTexture("blueportal.png");
          portal.color = pLight.color = Portal::BLUE_COLOR;
        } else {
          world.destinations["portal2"] = destination;
          PortalTeleport::setAction(*portal.trigger, "portal1");
          portal.overlayTex.diffuse = TextureLoader::getTexture("orangeportal.png");
          portal.color = pLight.color = Portal::ORANGE_COLOR;
        }
        // if both portals are open add the uncollider
        Entity &pEnt1 = *pPair.first;
        Entity &pEnt2 = *pPair.second;
        Portal &portal1 = dynamic_cast<Portal&>(pEnt1);
        Portal &portal2 = dynamic_cast<Portal&>(pEnt2);
        if (portal1.open && portal2.open) {
          portal1.addUncollider();
          portal2.addUncollider();
        }
      }
    }
  }
}

EntityPair& WorldHelper::getPortalPair(int pair, World &world) {
  if (pair > ((int) world.entityPairs.at("portalPairs").size()) - 1) {
    Portal *portal1 = &world.entityManager.create<Portal>();
    Portal *portal2 = &world.entityManager.create<Portal>();
    EntityPair pair = std::make_pair(portal1, portal2);
    world.entityPairs.at("portalPairs").push_back(pair);
  }
  return world.entityPairs.at("portalPairs").at(pair);
}

void WorldHelper::closePortals(World &world) {
  EntityPair &pPair = getPortalPair(0, world);
  Portal *portal1 = dynamic_cast<Portal*>(pPair.first);
  Portal *portal2 = dynamic_cast<Portal*>(pPair.second);

  portal1->open = false;
  portal2->open = false;
  
  portal1->trigger->remove();
  portal2->trigger->remove();
  
  portal1->removeUncollider();
  portal2->removeUncollider();
}

} /* namespace glPortal */
