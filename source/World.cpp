#include "World.hpp"

#include <SDL2/SDL_timer.h>
#include <climits>
#include <cmath>
#include <vector>
#include <cstdio>

#include <assets/map/MapLoader.hpp>
#include <assets/map/MapListLoader.hpp>
#include <assets/scene/SceneHelper.hpp>
#include <assets/scene/Scene.hpp>

#include <engine/renderer/Renderer.hpp>
#include <engine/env/Environment.hpp>
#include <engine/BoxCollider.hpp>

#include <engine/component/Health.hpp>
#include <engine/component/Transform.hpp>
#include <engine/component/Trigger.hpp>
#include <engine/component/AACollisionBox.hpp>
#include <engine/component/SoundSource.hpp>
#include <engine/component/LightSource.hpp>
#include "PlayerMotion.hpp"

#include <engine/core/math/Math.hpp>
#include <engine/core/math/Vector2f.hpp>
#include <engine/core/math/Vector3f.hpp>

#include <engine/system/JavascriptSystem.hpp>
#include <engine/core/state/GameState.hpp>
#include <SDL2/SDL_keyboard.h>

#include "Input.hpp"
#include "Portal.hpp"
#include "Window.hpp"
#include "WorldHelper.hpp"

namespace glPortal {

float World::gravity = GRAVITY;

World::World() :
  lastUpdateTime(0),
  scene(nullptr),
  renderer(nullptr),
  gameTime(0),
  config(Environment::getConfig()) {
  stateFunctionStack.push(&GameState::handleRunning);
  stateFunctionStack.push(&GameState::handleSplash);
}

void World::create() {
  mapList = MapListLoader::getMapList();
  renderer = new Renderer();
  js = new JavascriptSystem();
  lastUpdateTime = SDL_GetTicks();

  std::random_device rd;
  generator = std::mt19937(rd());

  std::string mapPath = config.mapPath;
  if (mapPath.length() > 0) {
    loadSceneFromPath(mapPath);
    return;
  }

  try {
    std::string map = config.map;
    loadScene(map);
    System::Log(Info) << "Custom map loaded";
  } catch (const std::out_of_range& e) {
    System::Log(Info) << "No custom map found loading default map.";
    loadScene(mapList[currentLevel]);
  }

}

void World::setRendererWindow(Window *win) {
  renderer->setViewport(win);
}

void World::destroy() {
  delete renderer;
  delete scene;
  delete js;
}

void World::loadScene(const std::string &path) {
  // Delete previous scene
  delete scene;
  currentScenePath = path;
  scene = MapLoader::getScene(path);

  Environment::dispatcher.dispatch(Event::loadScene);
  renderer->setScene(scene);
}

void World::loadSceneFromPath(const std::string &path) {
  delete scene;
  currentScenePath = path;
  scene = MapLoader::getSceneFromPath(path);
  Environment::dispatcher.dispatch(Event::loadScene);
  renderer->setScene(scene);
}

void World::update() {
  uint32_t updateTime = SDL_GetTicks();
  float dtime = (updateTime-lastUpdateTime)/1000.f;
  Entity &player = scene->player;
  Health &plrHealth = player.getComponent<Health>();
  Transform &plrTform = player.getComponent<Transform>();
  PlayerMotion &plrMotion = player.getComponent<PlayerMotion>();

  // Check if player is still alive
  if (not plrHealth.isAlive()) {
    plrTform.position = scene->start.getComponent<Transform>().position;
    plrHealth.revive();
    hidePortals();
  }

  // Calculate the view and new velocity of the player
  // FIXME: don't do this here, let a manager handle
  plrMotion.mouseLook();
  plrMotion.move(dtime);

  // Figure out the provisional new player position
  Vector3f pos = plrTform.position + plrMotion.velocity;

  //Y collision
  BoxCollider bboxY(Vector3f(plrTform.position.x, pos.y, plrTform.position.z), plrTform.scale);

  if (collidesWithWalls(bboxY)) {
    bool portaling = WorldHelper::isPlayerPortalingY(bboxY, &player, scene);

    if (not portaling and not plrMotion.noclip) {
      if (plrMotion.velocity.y < 0) {
        if (plrMotion.velocity.y < -HURT_VELOCITY) {
          std::uniform_int_distribution<> dis(0, PLAYER_FALL_SOUND.size()-1);
          player.getComponent<SoundSource>().playSound(
            Environment::getDataDir() + PLAYER_FALL_SOUND[dis(generator)]);
        }
        plrMotion.grounded = true;
      }
      plrMotion.velocity.y = 0;
    }
  } else {
    plrMotion.grounded = false;
  }

  //X collision
  BoxCollider bboxX(Vector3f(pos.x, plrTform.position.y, plrTform.position.z), plrTform.scale);
  if (collidesWithWalls(bboxX)) {
    bool portaling = WorldHelper::isPlayerPortalingX(bboxX, &player, scene);
    if (not portaling and not plrMotion.noclip) {
      plrMotion.velocity.x = 0;
    }
  }

  //Z collision
  BoxCollider bboxZ(Vector3f(plrTform.position.x, plrTform.position.y, pos.z), plrTform.scale);
  if (collidesWithWalls(bboxZ)) {
    bool portaling = WorldHelper::isPlayerPortalingZ(bboxZ, &player, scene);
    if (not portaling and not plrMotion.noclip) {
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
          System::Log(Verbose) << "Radiation touched.";
        } else if (trigger.type == "death") {
          player.getComponent<Health>().kill();
          System::Log(Verbose) << "Death touched.";
        } else if (trigger.type == "win") {
          if (currentLevel + 1 < mapList.size()) {
            currentLevel++;
            loadScene(mapList[currentLevel]);
          } else {
            scene->screen->enabled = true;
          }

          System::Log(Verbose) << "Win touched.";
        } else if (trigger.type == "map") {
          System::Log(Verbose) << "Map trigger touched.";
          throw __FILE__ ": Map trigger type de-implemented, please reimplement";
          //loadScene(trigger.reference);
        } else if (trigger.type == "button") {
          System::Log(Verbose) << "Button touched.";
          printf("Button touched\n");
        } else {
          System::Log(Verbose) << "Other trigger touched. " << trigger.type;
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
    if (currentLevel + 1 < mapList.size()) {
      currentLevel++;
      loadScene(mapList[currentLevel]);
    } else {
      scene->screen->enabled = true;
      scene->screen->title   = "Game Over";
      scene->screen->text    = "Hit q to quit the game.";
    }
  }

  lastUpdateTime = updateTime;
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
  WorldHelper::shootPortal(button, scene);
}

void World::render() {
  renderer->render();
}

Entity& World::getPlayer() {
  return scene->player;
}

void World::hidePortals() {
  WorldHelper::hidePortals(scene);
}

} /* namespace glPortal */
