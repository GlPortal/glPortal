#include "WorldHelper.hpp"
#include <assets/scene/SceneHelper.hpp>
#include <climits>
#include <engine/component/AACollisionBox.hpp>
#include <engine/component/MeshDrawable.hpp>
#include <engine/Ray.hpp>
#include <assets/texture/TextureLoader.hpp>
#include <SDL2/SDL_timer.h>

namespace glPortal {
void WorldHelper::shootPortal(int button, Scene *scene) {
  //Shooting
  Vector3f cameraDir = Math::toDirection(scene->camera.getRotation());

  //Find the closest intersection
  const Entity *closestWall = nullptr;
  float intersection = INT_MAX;
  for (const Entity &e : scene->entities) {
    // FIXME: Collision + MeshDrawable isn't the sole criteria we want to check
    if (e.hasComponent<AACollisionBox>() and e.hasComponent<MeshDrawable>()) {
      Ray bullet(scene->camera.getPosition(), cameraDir);
      float tNear, tFar;
      if (bullet.collides(e.getComponent<AACollisionBox>().box, &tNear, &tFar)) {
        if (tNear < intersection) {
          closestWall = &e;
          intersection = tNear;
        }
      }
    }
  }

  EntityPair &pPair = SceneHelper::getPortalPairFromScene(0, scene);
  // TODO: material in separate Component, + 1 mat per face
  if (closestWall != nullptr and (closestWall->getComponent<MeshDrawable>().material.portalable)) {
    Vector3f ipos = scene->camera.getPosition() + (cameraDir * intersection);
    Entity &pEnt = (button == 1) ? *pPair.first : *pPair.second;
    Portal &portal = pEnt.getComponent<Portal>();
    portal.openSince = SDL_GetTicks();
    portal.maskTex.diffuse = TextureLoader::getTexture("portalmask.png");
    portal.placeOnWall(scene->camera.getPosition(), closestWall->getComponent<AACollisionBox>().box, ipos);
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

void WorldHelper::hidePortals(Scene *scene) {
  EntityPair &pPair = SceneHelper::getPortalPairFromScene(0, scene);

  pPair.first->getComponent<Portal>().open = false;
  pPair.second->getComponent<Portal>().open = false;
}
} /* namespace glPortal */
