#include "WorldHelper.hpp"

#include <climits>
#include <SDL2/SDL_timer.h>
#include <assets/scene/SceneHelper.hpp>
#include <assets/texture/TextureLoader.hpp>
#include <engine/component/AACollisionBox.hpp>
#include <engine/component/MeshDrawable.hpp>
#include <engine/Ray.hpp>
#include "World.hpp"

namespace glPortal {
void WorldHelper::shootPortal(int button, Scene *scene) {
  //Shooting
  Vector3f cameraDir = Math::toDirection(scene->camera.getRotation());
  btVector3 btFrom = scene->camera.getPosition();
  btVector3 btTo = btFrom + cameraDir*10000;
  btCollisionWorld::ClosestRayResultCallback res(btFrom, btTo);

  scene->physics.world->rayTest(btFrom, btTo, res);

  if (res.hasHit()) {
    const Entity *pEnt = reinterpret_cast<Entity*>(res.m_collisionObject->getUserPointer());
    // All RigidBodies should have their pointer set, but check anyway
    if (pEnt) {
      const Entity &ent = *pEnt;
      // TODO: material in separate Component, + 1 mat per face
      if (ent.hasComponent<MeshDrawable>() and ent.getComponent<MeshDrawable>().material.portalable) {
        EntityPair &pPair = SceneHelper::getPortalPairFromScene(0, scene);
        Vector3f ipos(res.m_hitPointWorld);
        Entity &pEnt = (button == 1) ? *pPair.first : *pPair.second;
        Portal &portal = pEnt.getComponent<Portal>();
        portal.openSince = scene->world->getTime();
        portal.maskTex.diffuse = TextureLoader::getTexture("portalmask.png");
        // TODO: ditch AACollisionBoxes
        portal.placeOnWall(scene->camera.getPosition(), ent.getComponent<AACollisionBox>().box, ipos);
        LightSource &pLight = pEnt.getComponent<LightSource>();

        if (button == 1) {
          portal.overlayTex.diffuse = TextureLoader::getTexture("blueportal.png");
          portal.color = pLight.color = Portal::BLUE_COLOR;
        } else {
          portal.overlayTex.diffuse = TextureLoader::getTexture("orangeportal.png");
          portal.color = pLight.color = Portal::ORANGE_COLOR;
        }
      }
    }
  }
}

bool WorldHelper::isPlayerPortalingX(BoxCollider &box, Entity *player, Scene *scene) {
  bool portaling = false;
  for (const EntityPair &p : scene->portalPairs) {
    const Portal &portal1 = p.first->getComponent<Portal>(),
      &portal2 = p.second->getComponent<Portal>();
    if (portal1.open and portal2.open) {
      const Transform &p1Tform = p.first->getComponent<Transform>(),
        &p2Tform = p.second->getComponent<Transform>();
      if (portal1.inPortal(box)) {
        if (p1Tform.orientation.toAxAngle().x == 0 and (p1Tform.orientation.toAxAngle().y == rad(-90) || p1Tform.orientation.toAxAngle().y == rad(90))) {
          portaling = true;
        }
      }
      if (portal2.inPortal(box)) {
        if (p2Tform.orientation.toAxAngle().x == 0 and (p2Tform.orientation.toAxAngle().y == rad(-90) || p2Tform.orientation.toAxAngle().y == rad(90))) {
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
        if (p1Tform.orientation.toAxAngle().x == rad(-90) || p1Tform.orientation.toAxAngle().x == rad(90)) {
          portaling = true;
        }
      }
      if (portal2.inPortal(box)) {
        if (p2Tform.orientation.toAxAngle().x == rad(-90) || p2Tform.orientation.toAxAngle().x == rad(90)) {
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
        if (p1Tform.orientation.toAxAngle().x == 0 and (p1Tform.orientation.toAxAngle().y == 0 || p1Tform.orientation.toAxAngle().y == rad(180))) {
          portaling = true;
        }
      }
      if (portal2.inPortal(box)) {
        if (p2Tform.orientation.toAxAngle().x == 0 and (p2Tform.orientation.toAxAngle().y == 0 || p2Tform.orientation.toAxAngle().y == rad(180))) {
          portaling = true;
        }
      }
    }
  }
  return portaling;
}

void WorldHelper::hidePortals(Scene *scene) {
  EntityPair &pPair = SceneHelper::getPortalPairFromScene(0, scene);

  pPair.first->getComponent<Portal>().open = false;
  pPair.second->getComponent<Portal>().open = false;
}
} /* namespace glPortal */
