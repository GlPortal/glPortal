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
#include <engine/trigger/Trigger.hpp>
#include <engine/Light.hpp>
#include <engine/Ray.hpp>
#include <engine/Camera.hpp>
#include <engine/SoundManager.hpp>

#include <engine/core/math/Math.hpp>
#include <engine/core/math/Vector2f.hpp>
#include <engine/core/math/Vector3f.hpp>

#include "Input.hpp"
#include "Player.hpp"
#include "Portal.hpp"
#include "Window.hpp"

namespace glPortal {

float World::gravity = GRAVITY;
float World::friction = FRICTION;

World::World() : scene(nullptr) {
  config = Environment::getConfigPointer();
}

void World::create() {
  mapList = MapListLoader::getMapList();
  renderer = new Renderer();
  try {
    std::string map = config->getStringByKey(Config::MAP);
    loadScene(map);
    std::cout << "Custom map loaded.";
  } catch (const std::invalid_argument& e) {
    loadScene(mapList[currentLevel]);
  }

  std::random_device rd;
  generator =  std::mt19937(rd());
}

void World::setRendererWindow(Window *win) {
  renderer->setViewport(win);
}

void World::destroy() {
  delete renderer;
  delete scene;
}

void World::loadScene(const std::string &name) {
  // Delete previous scene
  delete scene;
  scene = MapLoader::getScene(name);
  //play a random piece of music each team a scene is loaded
  std::uniform_int_distribution<> dis(0, MUSIC_PLAYLIST.size()-1);
  SoundManager::PlayMusic(Environment::getDataDir() + MUSIC_PLAYLIST[dis(generator)]);
}

void World::update() {
  Player &player = scene->player;

  // Check if player is still alive
  if (not player.isAlive()) {
    player.position.set(scene->start.position);
    player.heal(100);
  }

  player.mouseLook();
  player.move();

  Vector3f pos = add(player.position, player.velocity);

  //FIXME Remake the collision system to be less faulty and ugly
  //Y collision
  BoxCollider bboxY(Vector3f(player.position.x, pos.y, player.position.z), player.scale);
  if (collidesWithWalls(bboxY)) {
    bool portaling = false;
    if (scene->bluePortal.open and scene->orangePortal.open) {
      if(scene->bluePortal.inPortal(bboxY)) {
        if(scene->bluePortal.rotation.x == -90 || scene->bluePortal.rotation.x == 90) {
          portaling = true;
        }
      }
      if(scene->orangePortal.inPortal(bboxY)) {
        if(scene->orangePortal.rotation.x == -90 || scene->orangePortal.rotation.x == 90) {
          portaling = true;
        }
      }
    }
    if(!portaling) {
      if (player.velocity.y < 0) {
        if(player.velocity.y < -0.18f) {
          std::uniform_int_distribution<> dis(0, PLAYER_FALL_SOUND.size()-1);
          SoundManager::PlaySound(Environment::getDataDir() + PLAYER_FALL_SOUND[dis(generator)], &player, SoundManager::PRIMARY);
        }

        player.grounded = true;
      }
      player.velocity.y = 0;
    }
  }

  //X collision
  BoxCollider bboxX(Vector3f(pos.x, player.position.y, player.position.z), player.scale);
  if (collidesWithWalls(bboxX)) {
    bool portaling = false;
    if (scene->bluePortal.open and scene->orangePortal.open) {
      if(scene->bluePortal.inPortal(bboxX)) {
        if(scene->bluePortal.rotation.x == 0 and (scene->bluePortal.rotation.y == -90 || scene->bluePortal.rotation.y == 90)) {
          portaling = true;
        }
      }
      if(scene->orangePortal.inPortal(bboxX)) {
        if(scene->bluePortal.rotation.x == 0 and (scene->orangePortal.rotation.y == -90 || scene->orangePortal.rotation.y == 90)) {
          portaling = true;
        }
      }
    }
    if(!portaling) {
      player.velocity.x = 0;
    }
  }

  //Z collision
  BoxCollider bboxZ(Vector3f(player.position.x, player.position.y, pos.z), player.scale);
  if (collidesWithWalls(bboxZ)) {
    bool portaling = false;
    
    if (scene->bluePortal.open and scene->orangePortal.open) {
      if(scene->bluePortal.inPortal(bboxZ)) {
        if(scene->bluePortal.rotation.x == 0 and (scene->bluePortal.rotation.y == 0 || scene->bluePortal.rotation.y == 180)) {
          portaling = true;
        }
      }
      if(scene->orangePortal.inPortal(bboxZ)) {
        if(scene->orangePortal.rotation.x == 0 and (scene->orangePortal.rotation.y == 0 || scene->orangePortal.rotation.y == 180)) {
          portaling = true;
        }
      }
    }
    if(!portaling) {
      player.velocity.z = 0;
    }
  }

  // Acids
  for (unsigned int i = 0; i < scene->volumes.size(); i++) {
    const PhysicsEntity &acid = scene->volumes[i];
    const BoxCollider playerCollider(player.position, player.scale);
    BoxCollider acidCollider(acid.position, acid.scale);

    if (playerCollider.collidesWith(acidCollider)) {
      player.kill();
    }
  }

  // Trigger
  for (unsigned int i = 0; i < scene->triggers.size(); i++) {
    const Trigger &trigger = scene->triggers[i];
    BoxCollider playerCollider(player.position, player.scale);
    BoxCollider triggerCollider(trigger.position, trigger.scale);

    if (playerCollider.collidesWith(triggerCollider)) {
      if (trigger.type == "radiation") {
        player.harm(10);
      } else if (trigger.type == "death") {
        player.kill();
        printf("Death touched\n");
      } else if (trigger.type == "win") {
        if(currentLevel + 1 < mapList.size()) {
          currentLevel++;
        }
        loadScene(mapList[currentLevel]);
        printf("Win touched\n");
      } else if (trigger.type == "map") {
        printf("Map Trigger touched\n");
        loadScene(trigger.reference);
      } else if (trigger.type == "button") {
        printf("Button touched\n");
      } else {
        printf("Some trigger touched: %s\n", trigger.type.c_str());
      }
    }
  }

  pos = player.position + player.velocity;

  //Check if the player is moving through a portal
  BoxCollider playerCollider(pos, player.scale);
  if (scene->bluePortal.open and scene->orangePortal.open) {
    if (scene->bluePortal.throughPortal(playerCollider)) {
      player.position.set(scene->orangePortal.position);
      float rotation = scene->orangePortal.rotation.y - scene->bluePortal.rotation.y + 180;
      player.rotation.y += rotation;
      //Transform the velocity of the player
      float velocity = player.velocity.length();
      player.velocity = *scene->orangePortal.getDirection().scale(velocity);
    }
    if (scene->orangePortal.throughPortal(playerCollider)) {
      player.position.set(scene->bluePortal.position);
      float rotation = scene->bluePortal.rotation.y - scene->orangePortal.rotation.y + 180;
      player.rotation.y += rotation;
      //Transform the velocity of the player
      float velocity = player.velocity.length();
      player.velocity = *scene->bluePortal.getDirection().scale(velocity);
    }
  }

  //Add velocity to the player position
  player.position.add(player.velocity);

  //Parent camera to player
  scene->camera.position.set(scene->player.position);
  scene->camera.position.y += scene->player.scale.y/2;
  scene->camera.rotation.set(scene->player.rotation);

  //Check if the end of the level has been reached
  float distToEnd = (scene->end.position - scene->player.position).length();
  if (distToEnd < 1) {
    if(currentLevel + 1 < mapList.size()) {
      currentLevel++;
    }
    loadScene(mapList[currentLevel]);
  }
}

bool World::collidesWithWalls(const BoxCollider &collider) const {
  for (unsigned int i = 0; i < scene->walls.size(); i++) {
    const PhysicsEntity &wall = scene->walls[i];
    const BoxCollider &wallCollider = wall.physBody;

    if (collider.collidesWith(wallCollider)) {
      return true;
    }
  }
  return false;
}

void World::shootPortal(int button) {
  //Shooting
  Vector3f cameraDir = Math::toDirection(scene->camera.rotation);

  //Find the closest intersection
  PhysicsEntity *closestWall = nullptr;
  float intersection = INT_MAX;
  for (unsigned int i = 0; i < scene->walls.size(); ++i) {
    PhysicsEntity &wall = scene->walls[i];
    Ray bullet(scene->camera.position, cameraDir);
    float tNear, tFar;
    if (bullet.collides(wall, &tNear, &tFar)) {
      if (tNear < intersection) {
        closestWall = &wall;
        intersection = tNear;
      }
    }
  }

  if (closestWall != nullptr and (closestWall->material.portalable)) {
    BoxCollider wall(closestWall->position, closestWall->scale);
    Vector3f ipos = scene->camera.position + (cameraDir * intersection);
    Portal portal;
    portal.openSince = SDL_GetTicks();
    portal.maskTex.diffuse = TextureLoader::getTexture("portalmask.png"); 
    portal.placeOnWall(wall, ipos);

    if (button == 1) {
      portal.material.diffuse = TextureLoader::getTexture("blueportal.png");
      portal.color = Portal::BLUE_COLOR;
      scene->bluePortal = portal;
    } else {
      portal.material.diffuse = TextureLoader::getTexture("orangeportal.png");
      portal.color = Portal::ORANGE_COLOR;
      scene->orangePortal = portal;
    }
  } else {
    if (button == 1) {
      scene->bluePortal.open = false;
    } else {
      scene->orangePortal.open = false;
    }
  }
}

void World::render() {
  renderer->setScene(scene);
  renderer->render();
}

Player *World::getPlayer() {
  return &scene->player;
}

} /* namespace glPortal */
