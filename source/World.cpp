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
#include "engine/renderer/Renderer.hpp"
#include "engine/Texture.hpp"
#include "engine/loader/TextureLoader.hpp"
#include "engine/Trigger.hpp"
#include <util/math/Math.hpp>
#include <util/math/Vector2f.hpp>
#include <util/math/Vector3f.hpp>
#include "Input.hpp"
#include "Player.hpp"
#include "Portal.hpp"
#include "Scene.hpp"

namespace glPortal {

const Vector2f sensitivity(0.25, 0.25);
const float SPEED = 0.1f;
const float GRAVITY = 0.01f;
const float FRICTION = 0.01f;
const float JUMP_SPEED = 0.15f;

void World::create() {
  renderer = new Renderer();

    loadScene("/maps/n3.xml");
    //loadScene("/maps/untitled.xml");
}

void World::destroy() {
  delete (renderer);
  delete (scene);
}

void World::loadScene(std::string path) {
  scene = MapLoader::getScene(path);
}

void World::update() {
  //Mouselook
  int mousedx, mousedy;
  SDL_GetRelativeMouseState(&mousedx, &mousedy);

  Player* player = &scene->player;
  // Apply mouse movement to view
  player->rotation.x -= mousedy * sensitivity.x;
  player->rotation.y -= mousedx * sensitivity.y;

  // Restrict rotation in horizontal axis
  if (player->rotation.x < -90) {
    player->rotation.x = -90;
  }
  if (player->rotation.x > 90) {
    player->rotation.x = 90;
  }

  if (player->velocity.x >= FRICTION) {
    player->velocity.x -= FRICTION;
  }
  if (player->velocity.x <= -FRICTION) {
    player->velocity.x += FRICTION;
  }
  if (player->velocity.z >= FRICTION) {
    player->velocity.z -= FRICTION;
  }
  if (player->velocity.z <= -FRICTION) {
    player->velocity.z += FRICTION;
  }
  if (player->velocity.x < FRICTION && player->velocity.x > -FRICTION) {player->velocity.x = 0;}
  if (player->velocity.z < FRICTION && player->velocity.z > -FRICTION) {player->velocity.z = 0;}
  player->velocity.y -= GRAVITY;

  float rotation = Math::toRadians(player->rotation.y);
  if (player->isAlive()){
    if (Input::isKeyDown('w')) {
      player->velocity.x = -sin(rotation) * SPEED;
      player->velocity.z = -cos(rotation) * SPEED;
    }
    if (Input::isKeyDown('s')) {
      player->velocity.x = sin(rotation) * SPEED;
      player->velocity.z = cos(rotation) * SPEED;
    }
    if (Input::isKeyDown('a')) {
      player->velocity.x = -cos(rotation) * SPEED;
      player->velocity.z = sin(rotation) * SPEED;
    }
    if (Input::isKeyDown('d')) {
      player->velocity.x = cos(rotation) * SPEED;
      player->velocity.z = -sin(rotation) * SPEED;
    }
    if (Input::isKeyDown(' ') && player->grounded) {
      player->grounded = false;
      player->velocity.y = JUMP_SPEED;
    }
  }

  Vector3f pos = Vector3f::add(player->position, player->velocity);

  //Check if the player is walking through a portal
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

  collidePlayer();

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
  Vector3f cameraDir = scene->camera.getDirection();

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

void World::collidePlayer() {
  Player* player = &scene->player;
  Vector3f futurePlayerPosition = Vector3f::add(player->position, player->velocity);

  BoxCollider colliderX(Vector3f(futurePlayerPosition.x, player->position.y, player->position.z),
                        player->scale);
  BoxCollider colliderY(Vector3f(player->position.x, futurePlayerPosition.y, player->position.z),
                        player->scale);
  BoxCollider colliderZ(Vector3f(player->position.x, player->position.y, futurePlayerPosition.z),
                        player->scale);

  if (collidesWithWallOutsidePortal(colliderX)) {
    player->velocity.x = 0;
  }

  if (collidesWithWallOutsidePortal(colliderY)) {
    if (player->velocity.y < 0) {
      player->grounded = true;
    }
    player->velocity.y = 0;
  }

  if (collidesWithWallOutsidePortal(colliderZ)) {
    player->velocity.z = 0;
  }
}

bool World::collidesWithWallOutsidePortal(BoxCollider collider) {
  if (collidesWithWalls(collider)) {
    if (not(scene->bluePortal.open && scene->orangePortal.open &&
            (scene->bluePortal.inPortal(collider) || scene->orangePortal.inPortal(collider)))) {
      return true;
    }
  }
  return false;
}

} /* namespace glPortal */
