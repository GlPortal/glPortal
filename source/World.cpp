#include "World.hpp"

#include <SDL2/SDL_mouse.h>
#include <climits>
#include <cmath>
#include <vector>
#include <cstdio>

#include "engine/BoxCollider.hpp"
#include "engine/Camera.hpp"
#include "engine/env/Environment.hpp"
#include "engine/Light.hpp"
#include "engine/loader/MapLoader.hpp"
#include "engine/Mesh.hpp"
#include "engine/loader/MeshLoader.hpp"
#include "engine/Ray.hpp"
#include "engine/Renderer.hpp"
#include "engine/Texture.hpp"
#include "engine/loader/TextureLoader.hpp"
#include "engine/Trigger.hpp"
#include "engine/util/Math.hpp"
#include "engine/util/Vector2f.hpp"
#include "engine/util/Vector3f.hpp"
#include "Input.hpp"
#include "Player.hpp"
#include "Portal.hpp"
#include "Scene.hpp"

namespace glPortal {

float World::gravity = GRAVITY;
float World::friction = FRICTION;

void World::create() {
  renderer = new Renderer();

  loadScene("/maps/n3.xml");
}

void World::destroy() {
  delete (renderer);
  delete (scene);
}

void World::loadScene(std::string path) {
  scene = MapLoader::getScene(path);
}

void World::update() {
  Player* player = &scene->player;

  player->mouseLook();
  player->move();

  Vector3f pos = Vector3f::add(player->position, player->velocity);

  //Check if the player is moving through a portal
  BoxCollider playerCollider(pos, player->scale);
  if (scene->bluePortal.open && scene->orangePortal.open) {
    if (scene->bluePortal.throughPortal(playerCollider)) {
      player->position.set(scene->orangePortal.position);
      float rotation = scene->orangePortal.rotation.y - scene->bluePortal.rotation.y + 180;
      player->rotation.y += rotation;
      //Transform the velocity of the player
      float velocity = player->velocity.length();
      player->velocity = *scene->orangePortal.getDirection().scale(velocity);
    }
    if (scene->orangePortal.throughPortal(playerCollider)) {
       player->position.set(scene->bluePortal.position);
       float rotation = scene->bluePortal.rotation.y - scene->orangePortal.rotation.y + 180;
       player->rotation.y += rotation;
       //Transform the velocity of the player
       float velocity = player->velocity.length();
       player->velocity = *scene->bluePortal.getDirection().scale(velocity);
    }
  }

  pos = Vector3f::add(player->position, player->velocity);

  //Y collision
  BoxCollider bboxY(Vector3f(player->position.x, pos.y, player->position.z), player->scale);
  if (collidesWithWalls(bboxY)) {
    if (scene->bluePortal.open && scene->orangePortal.open &&
      (scene->bluePortal.inPortal(bboxY) || scene->orangePortal.inPortal(bboxY))) {

    } else {
      if (player->velocity.y < 0) {
        player->grounded = true;
      }
      player->velocity.y = 0;
    }
  }

  //X collision
  BoxCollider bboxX(Vector3f(pos.x, player->position.y, player->position.z), player->scale);
  if (collidesWithWalls(bboxX)) {
    if (scene->bluePortal.open && scene->orangePortal.open &&
      (scene->bluePortal.inPortal(bboxX) || scene->orangePortal.inPortal(bboxX))) {

    } else {
      player->velocity.x = 0;
    }
  }

  //Z collision
  BoxCollider bboxZ(Vector3f(player->position.x, player->position.y, pos.z), player->scale);
  if (collidesWithWalls(bboxZ)) {
    if (scene->bluePortal.open && scene->orangePortal.open &&
      (scene->bluePortal.inPortal(bboxZ) || scene->orangePortal.inPortal(bboxZ))) {

    } else {
      player->velocity.z = 0;
    }
  }

  //Trigger
  for (unsigned int i = 0; i < scene->triggers.size(); i++) {
    Trigger trigger = scene->triggers[i];
    BoxCollider playerCollider(player->position, player->scale);
    BoxCollider triggerCollider(trigger.position, trigger.scale);

    if (playerCollider.collidesWith(triggerCollider)) {
      if (trigger.type == "radiation") {
        player->harm(10);
      } else if (trigger.type == "death") {
        player->kill();
        printf("Death touched\n");
      } else if (trigger.type == "win") {
        printf("Win touched\n");
      } else {
        printf("Some trigger touched: %s\n", trigger.type.c_str());
      }
    }
  }

  //Add velocity to the player position
  player->position.add(player->velocity);

  //Parent camera to player
  scene->camera.position.set(scene->player.position);
  scene->camera.position.y += scene->player.scale.y/2;
  scene->camera.rotation.set(scene->player.rotation);

  //Check if the end of the level has been reached
  float distToEnd = Vector3f::sub(scene->end.position, scene->player.position).length();
  if (distToEnd < 1) {
    //FIXME Load the next scene
    loadScene("/maps/n2.xml");
  }
}

bool World::collidesWithWalls(BoxCollider collider) {
  for (unsigned int i = 0; i < scene->walls.size(); i++) {
    Entity wall = scene->walls[i];
    BoxCollider wallCollider(wall.position, wall.scale);

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
  Entity closestWall;
  float intersection = INT_MAX;
  for (unsigned int i = 0; i < scene->walls.size(); i++) {
    Entity wall = scene->walls[i];
    Ray bullet = Ray(scene->camera.position, cameraDir);
    float tNear, tFar;
    if (bullet.collides(wall, &tNear, &tFar)) {
      if (wall.texture.handle == TextureLoader::getTexture("wall.png").handle) {
        if (tNear < intersection) {
          closestWall = wall;
          intersection = tNear;
        }
      }
    }
  }

  BoxCollider wall(closestWall.position, closestWall.scale);
  Vector3f ipos = Vector3f::add(scene->camera.position, Vector3f::scale(cameraDir, intersection));
  Portal portal;
  portal.placeOnWall(wall, ipos);

  if (button == 1) {
    portal.texture = TextureLoader::getTexture("blueportal.png");
    scene->bluePortal = portal;
  } else {
    portal.texture = TextureLoader::getTexture("orangeportal.png");
    scene->orangePortal = portal;
  }
}

void World::render() {
  renderer->render(scene);
}

} /* namespace glPortal */
