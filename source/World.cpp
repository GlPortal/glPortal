#include "World.hpp"

#include <SDL2/SDL_mouse.h>
#include <SDL2/SDL_timer.h>
#include <climits>
#include <cmath>
#include <vector>
#include <cstdio>

#include <assets/map/MapLoader.hpp>
#include <assets/map/MapListLoader.hpp>
#include <assets/model/MeshLoader.hpp>
#include <assets/texture/TextureLoader.hpp>

#include <assets/scene/Scene.hpp>
#include <assets/model/Mesh.hpp>
#include <assets/texture/Texture.hpp>

#include <engine/renderer/Renderer.hpp>
#include <engine/env/Environment.hpp>
#include <engine/BoxCollider.hpp>
#include <engine/Ray.hpp>
#include <engine/Camera.hpp>
#include <engine/SoundManager.hpp>

#include <engine/component/Health.hpp>
#include <engine/component/Transform.hpp>
#include <engine/component/Trigger.hpp>
#include <engine/component/AACollisionBox.hpp>
#include <engine/component/MeshDrawable.hpp>
#include <engine/component/SoundSource.hpp>
#include <engine/component/LightSource.hpp>
#include "PlayerMotion.hpp"

#include <engine/core/math/Math.hpp>
#include <engine/core/math/Vector2f.hpp>
#include <engine/core/math/Vector3f.hpp>

#include <SDL2/SDL_keyboard.h>

#include "Editor.hpp"
#include "Input.hpp"
#include "Portal.hpp"
#include "Window.hpp"

namespace glPortal {

float World::gravity = GRAVITY;
float World::friction = FRICTION;

World::World() :
  scene(nullptr),
  isEditorShown(false) {
  config = Environment::getConfigPointer();
}

void World::create() {
  editor = new Editor(*this);
  mapList = MapListLoader::getMapList();
  renderer = new Renderer();
  try {
    std::string map = config->getString(Config::MAP);
    loadScene(map);
    System::Log(Info) << "Custom map loaded";
  } catch (const std::out_of_range& e) {
    printf("No custom map found loading default map.\n");
    loadScene(mapList[currentLevel]);
  }

  std::random_device rd;
  generator =  std::mt19937(rd());
}

void World::setRendererWindow(Window *win) {
  renderer->setViewport(win);
}

void World::destroy() {
  delete editor;
  delete renderer;
  delete scene;
}

void World::loadScene(const std::string &path) {
  // Delete previous scene
  delete scene;
  currentScenePath = path;
  scene = MapLoader::getScene(path);
  //play a random piece of music each team a scene is loaded
  std::uniform_int_distribution<> dis(0, MUSIC_PLAYLIST.size()-1);
  SoundManager::PlayMusic(Environment::getDataDir() + MUSIC_PLAYLIST[dis(generator)]);
}

void World::update() {
  // If F5 released, reload the scene
  if (wasF5Down and not Input::isKeyDown(SDL_SCANCODE_F5)) {
    if (Input::isKeyDown(SDL_SCANCODE_LSHIFT) || Input::isKeyDown(SDL_SCANCODE_RSHIFT)) {
      // Enable reload-on-change (inotify on Linux)
    }

    loadScene(currentScenePath);
  }
  wasF5Down = Input::isKeyDown(SDL_SCANCODE_F5);
  // If Tab released, toggle editor
  if (wasTabDown and not Input::isKeyDown(SDL_SCANCODE_TAB)) {
    isEditorShown = !isEditorShown;
  }
  wasTabDown = Input::isKeyDown(SDL_SCANCODE_F5);

  Entity &player = scene->player;
  Health &plrHealth = player.getComponent<Health>();
  Transform &plrTform = player.getComponent<Transform>();
  PlayerMotion &plrMotion = player.getComponent<PlayerMotion>();

  // Check if player is still alive
  if (not plrHealth.isAlive()) {
    plrTform.position = scene->start.getComponent<Transform>().position;
    plrHealth.revive();
  }

  // Calculate the view and new velocity of the player
  // FIXME: don't do this here, let a manager handle
  plrMotion.mouseLook();
  plrMotion.move();

  // Figure out the provisional new player position
  Vector3f pos = plrTform.position + plrMotion.velocity;

  //FIXME Remake the collision system to be less faulty and ugly
  // ^ A.K.A. Bullet physics engine

  //Y collision
  BoxCollider bboxY(Vector3f(plrTform.position.x, pos.y, plrTform.position.z), plrTform.scale);
  if (collidesWithWalls(bboxY)) {
    bool portaling = false;
    for (const EntityPair &p : scene->portalPairs) {
      const Portal &portal1 = p.first->getComponent<Portal>(),
                   &portal2 = p.second->getComponent<Portal>();
      if (portal1.open and portal2.open) {
        const Transform &p1Tform = p.first->getComponent<Transform>(),
                        &p2Tform = p.second->getComponent<Transform>();
        if (portal1.inPortal(bboxY)) {
          if (p1Tform.rotation.x == rad(-90) || p1Tform.rotation.x == rad(90)) {
            portaling = true;
          }
        }
        if (portal2.inPortal(bboxY)) {
          if (p2Tform.rotation.x == rad(-90) || p2Tform.rotation.x == rad(90)) {
            portaling = true;
          }
        }
      }
    }
    if(!portaling) {
      if (plrMotion.velocity.y < 0) {
        if(plrMotion.velocity.y < -HURT_VELOCITY) {
          std::uniform_int_distribution<> dis(0, PLAYER_FALL_SOUND.size()-1);
          player.getComponent<SoundSource>().playSound(
            Environment::getDataDir() + PLAYER_FALL_SOUND[dis(generator)]);
        }
        plrMotion.grounded = true;
      }
      plrMotion.velocity.y = 0;
    }
  }

  //X collision
  BoxCollider bboxX(Vector3f(pos.x, plrTform.position.y, plrTform.position.z), plrTform.scale);
  if (collidesWithWalls(bboxX)) {
    bool portaling = false;
    for (const EntityPair &p : scene->portalPairs) {
      const Portal &portal1 = p.first->getComponent<Portal>(),
                   &portal2 = p.second->getComponent<Portal>();
      if (portal1.open and portal2.open) {
        const Transform &p1Tform = p.first->getComponent<Transform>(),
                        &p2Tform = p.second->getComponent<Transform>();
        if (portal1.inPortal(bboxX)) {
          if (p1Tform.rotation.x == 0 and (p1Tform.rotation.y == rad(-90) || p1Tform.rotation.y == rad(90))) {
            portaling = true;
          }
        }
        if (portal2.inPortal(bboxX)) {
          if (p2Tform.rotation.x == 0 and (p2Tform.rotation.y == rad(-90) || p2Tform.rotation.y == rad(90))) {
            portaling = true;
          }
        }
      }
    }
    if(!portaling) {
      plrMotion.velocity.x = 0;
    }
  }

  //Z collision
  BoxCollider bboxZ(Vector3f(plrTform.position.x, plrTform.position.y, pos.z), plrTform.scale);
  if (collidesWithWalls(bboxZ)) {
    bool portaling = false;
    for (const EntityPair &p : scene->portalPairs) {
      const Portal &portal1 = p.first->getComponent<Portal>(),
                   &portal2 = p.second->getComponent<Portal>();
      if (portal1.open and portal2.open) {
        const Transform &p1Tform = p.first->getComponent<Transform>(),
                        &p2Tform = p.second->getComponent<Transform>();
        if (portal1.inPortal(bboxZ)) {
          if (p1Tform.rotation.x == 0 and (p1Tform.rotation.y == 0 || p1Tform.rotation.y == rad(180))) {
            portaling = true;
          }
        }
        if (portal2.inPortal(bboxZ)) {
          if (p2Tform.rotation.x == 0 and (p2Tform.rotation.y == 0 || p2Tform.rotation.y == rad(180))) {
            portaling = true;
          }
        }
      }
    }
    if(!portaling) {
      plrMotion.velocity.z = 0;
    }
  }

  for (Entity &e : scene->entities) {
    // Trigger
    if (e.hasComponent<Trigger>()) {
      const Trigger &trigger = e.getComponent<Trigger>();
      const Transform &trTform = e.getComponent<Transform>();
      BoxCollider playerCollider(plrTform.position, plrTform.scale);
      BoxCollider triggerCollider(trTform.position, trTform.scale);

      if (playerCollider.collidesWith(triggerCollider)) {
        if (trigger.type == "radiation") {
          player.getComponent<Health>().harm(.1f);
        } else if (trigger.type == "death") {
          player.getComponent<Health>().kill();
          printf("Death touched\n");
        } else if (trigger.type == "win") {
          if(currentLevel + 1 < mapList.size()) {
            currentLevel++;
          }
          loadScene(mapList[currentLevel]);
          printf("Win touched\n");
        } else if (trigger.type == "map") {
          printf("Map Trigger touched\n");
          throw __FILE__ ": Map trigger type de-implemented, please reimplement";
          //loadScene(trigger.reference);
        } else if (trigger.type == "button") {
          printf("Button touched\n");
        } else {
          printf("Some trigger touched: %s\n", trigger.type.c_str());
        }
      }
    }
  }

  pos = plrTform.position + plrMotion.velocity;

  // Check if the player is moving through a portal
  BoxCollider playerCollider(pos, plrTform.scale);
  for (EntityPair &p : scene->portalPairs) {
    Portal &portal1 = p.first->getComponent<Portal>(),
            &portal2 = p.second->getComponent<Portal>();
    Transform &p1Tform = p.first->getComponent<Transform>(),
              &p2Tform = p.second->getComponent<Transform>();
    if (portal1.open and portal2.open) {
      if (portal1.throughPortal(playerCollider)) {
        plrTform.position = p2Tform.position;
        float rotation = p2Tform.rotation.y - p1Tform.rotation.y + rad(180);
        plrTform.rotation.y += rotation;
        // Transform the velocity of the player
        float velocity = plrMotion.velocity.length();
        plrMotion.velocity = portal2.getDirection() * velocity;
      }
      if (portal2.throughPortal(playerCollider)) {
        plrTform.position = p1Tform.position;
        float rotation = p1Tform.rotation.y - p2Tform.rotation.y + rad(180);
        plrTform.rotation.y += rotation;
        // Transform the velocity of the player
        float velocity = plrMotion.velocity.length();
        plrMotion.velocity = portal1.getDirection() * velocity;
      }
    }
  }

  //Add velocity to the player position
  plrTform.position += plrMotion.velocity;

  //Parent camera to player
  scene->camera.setPerspective();
  int vpWidth, vpHeight;
  renderer->getViewport()->getSize(&vpWidth, &vpHeight);
  scene->camera.setAspect((float)vpWidth / vpHeight);
  scene->camera.setPosition(plrTform.position + Vector3f(0, plrTform.scale.y/2, 0));
  scene->camera.setRotation(plrTform.rotation);

  //Check if the end of the level has been reached
  float distToEnd = (scene->end.getComponent<Transform>().position - plrTform.position).length();
  if (distToEnd < 1) {
    if(currentLevel + 1 < mapList.size()) {
      currentLevel++;
    }
    loadScene(mapList[currentLevel]);
  }
}

bool World::collidesWithWalls(const BoxCollider &collider) const {
  for (const Entity &e : scene->entities) {
    if (e.hasComponent<AACollisionBox>()) {
      if (collider.collidesWith(e.getComponent<AACollisionBox>().box)) {
        return true;
      }
    }
  }
  return false;
}

void World::shootPortal(int button) {
  //Shooting
  Vector3f cameraDir = Math::toDirection(scene->camera.getRotation());

  //Find the closest intersection
  const Entity *closestWall = nullptr;
  float intersection = INT_MAX;
  for (const Entity &e : scene->entities) {
    // FIXME: Collsion + MeshDrawable isn't the sole criteria we want to check
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

  EntityPair &pPair = getPortalPair(0);
  // TODO: material in separate Component, + 1 mat per face
  if (closestWall != nullptr and (closestWall->getComponent<MeshDrawable>().material.portalable)) {
    Vector3f ipos = scene->camera.getPosition() + (cameraDir * intersection);
    Entity &pEnt = (button == 1) ? *pPair.first : *pPair.second;
    Portal &portal = pEnt.getComponent<Portal>();
    portal.openSince = SDL_GetTicks();
    portal.maskTex.diffuse = TextureLoader::getTexture("portalmask.png"); 
    portal.placeOnWall(scene->camera.getPosition(), closestWall->getComponent<AACollisionBox>().box, ipos);
    //LightSource &pLight = pEnt.getComponent<LightSource>();

    if (button == 1) {
      portal.overlayTex.diffuse = TextureLoader::getTexture("blueportal.png");
      portal.color /*= pLight.color*/ = Portal::BLUE_COLOR;
    } else {
      portal.overlayTex.diffuse = TextureLoader::getTexture("orangeportal.png");
      portal.color /*= pLight.color*/ = Portal::ORANGE_COLOR;
    }
  } else {
    if (button == 1) {
      pPair.first->getComponent<Portal>().open = false;
    } else {
      pPair.second->getComponent<Portal>().open = false;
    }
  }
}

void World::render() {
  renderer->setScene(scene);
  renderer->render(scene->camera);
  if (isEditorShown) {
    editor->renderUI();
  }
}

Entity& World::getPlayer() {
  return scene->player;
}

EntityPair& World::getPortalPair(int n) const {
  int pairCount = scene->portalPairs.size();
  if (pairCount <= n) {
    scene->portalPairs.reserve(n + 1);
    // Instantiate and initalize every non-extsing yet portal pair
    for (int i = pairCount; i <= n; ++i) {
      scene->entities.emplace_back();
      Entity &pEnt1 = scene->entities.back();
      scene->entities.emplace_back();
      Entity &pEnt2 = scene->entities.back();
      pEnt1.addComponent<Transform>();
      pEnt1.addComponent<Portal>();
      //LightSource &ls1 = pEnt1.addComponent<LightSource>();
      pEnt2.addComponent<Transform>();
      pEnt2.addComponent<Portal>();
      /*LightSource &ls2 = pEnt2.addComponent<LightSource>();
      ls1.energy = ls2.energy = 5;
      ls1.distance = ls2.distance = 1.3f;*/
      scene->portalPairs.emplace_back(&pEnt1, &pEnt2);
    }
  }
  return scene->portalPairs.at(n);
}

} /* namespace glPortal */
