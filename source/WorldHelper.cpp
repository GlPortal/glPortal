#include "WorldHelper.hpp"

#include <climits>
#include <SDL2/SDL_timer.h>
#include <assets/scene/SceneHelper.hpp>
#include <assets/texture/TextureLoader.hpp>
#include <engine/component/MeshDrawable.hpp>
#include "World.hpp"

namespace glPortal {

void WorldHelper::shootPortal(int button, Scene *scene) {
  Vector3f cameraDir = Math::toDirection(scene->camera.getOrientation());
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
      if (ent.hasComponent<MeshDrawable>() and
          ent.getComponent<MeshDrawable>().material.portalable) {
        EntityPair &pPair = SceneHelper::getPortalPairFromScene(0, scene);
        Vector3f ipos(res.m_hitPointWorld);
        Entity &pEnt = (button == 1) ? *pPair.first : *pPair.second;
        Portal &portal = pEnt.getComponent<Portal>();
        portal.openSince = scene->world->getTime();
        portal.maskTex.diffuse = TextureLoader::getTexture("portalmask.png");
        // TODO: ditch AACollisionBoxes
        portal.placeOnWall(scene->camera.getPosition(), ipos, res.m_hitNormalWorld);

        const Entity& otherPortalEntity = (button==1) ? *pPair.second : *pPair.first;
        Portal& otherPortal = otherPortalEntity.getComponent<Portal>();
        if(otherPortal.open)  {
          otherPortal.isUncolliderActive = true;
          portal.isUncolliderActive = true;
        }

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

void WorldHelper::closePortals(Scene *scene) {
  EntityPair &pPair = SceneHelper::getPortalPairFromScene(0, scene);

  pPair.first->getComponent<Portal>().open = false;
  pPair.second->getComponent<Portal>().open = false;
}

} /* namespace glPortal */
