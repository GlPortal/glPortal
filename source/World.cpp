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
#include <assets/scene/SceneHelper.hpp>

#include <assets/scene/Scene.hpp>
#include <assets/model/Mesh.hpp>
#include <assets/texture/Texture.hpp>

#include <engine/renderer/Renderer.hpp>
#include <engine/env/Environment.hpp>
#include <engine/BoxCollider.hpp>
#include <engine/Ray.hpp>
#include <engine/Camera.hpp>

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
#include <engine/core/event/Dispatcher.hpp>

#include <SDL2/SDL_keyboard.h>

#include "Editor.hpp"
#include "Input.hpp"
#include "Portal.hpp"
#include "Window.hpp"
#include "WorldHelper.hpp"

namespace glPortal {

float World::gravity = GRAVITY;
World::World() :
  isEditorShown(false),
  gameTime(0),
  scene(nullptr),
  config(Environment::getConfig()) {
}

void World::create() {
  editor = new Editor(*this);
  mapList = MapListLoader::getMapList();
  renderer = new Renderer();
  try {
    std::string map = config.map;
    loadScene(map);
    System::Log(Info) << "Custom map loaded";
  } catch (const std::out_of_range& e) {
    System::Log(Info) << "No custom map found loading default map.";
    loadScene(mapList[currentLevel]);
  }

  std::random_device rd;
  generator = std::mt19937(rd());
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

  // FIXME Shall we do this here? Likely not.
  scene->world = this;
  phys.setScene(scene);
  scene->physics.world->setDebugDrawer(&pdd);
  scene->physics.setGravity(0, -9.8, 0);

  Environment::dispatcher.dispatch(Event::loadScene);
}

double World::getTime() const {
  return gameTime;
}

void World::update(double dtime) {
  gameTime += dtime;
  
  Entity &player = *scene->player;
  Health &plrHealth = player.getComponent<Health>();
  Transform &plrTform = player.getComponent<Transform>();
  PlayerMotion &plrMotion = player.getComponent<PlayerMotion>();

  // Check if player is still alive
  if (not plrHealth.isAlive()) {
    plrTform.position = scene->start->getComponent<Transform>().position;
    plrHealth.revive();
    hidePortals();
  }

  // Calculate the view and new velocity of the player
  // FIXME: don't do this here, let a manager handle
  plrMotion.mouseLook();
  plrMotion.move(dtime);

  phys.update(dtime);
/*
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
          }
          loadScene(mapList[currentLevel]);
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
  BoxCollider playerCollider(pos, plrTform.scale);// TODO reimplement with Bullet
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
  }*/

  //Parent camera to player
  scene->camera.setPerspective();
  int vpWidth, vpHeight;
  renderer->getViewport()->getSize(&vpWidth, &vpHeight);
  scene->camera.setAspect((float)vpWidth / vpHeight);
  scene->camera.setPosition(plrTform.position + Vector3f(0, plrTform.scale.y/2, 0));
  scene->camera.setRotation(plrMotion.rotation);

  //Check if the end of the level has been reached
  float distToEnd = (scene->end->getComponent<Transform>().position - plrTform.position).length();
  if (distToEnd < 1) {
    if (currentLevel + 1 < mapList.size()) {
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
  WorldHelper::shootPortal(button, scene);
}

Renderer* World::getRenderer() const {
  return renderer;
}

void World::render(double dtime) {
  renderer->setScene(scene);
  renderer->render(dtime, scene->camera);
  if (isPhysicsDebugEnabled) {
    scene->physics.world->debugDrawWorld();
    pdd.render(scene->camera);
  }
  if (isEditorShown) {
    editor->renderUI();
  }
}

Entity& World::getPlayer() {
  return *scene->player;
}

void World::hidePortals() {
  WorldHelper::closePortals(scene);
}

} /* namespace glPortal */
