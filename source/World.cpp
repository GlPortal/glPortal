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

const Vector2f sensitivity(0.5, 0.5);
const float SPEED = 0.2;
const float GRAVITY = 0.05;

void World::create() {
  renderer = new Renderer();

    loadScene("/maps/n1.xml");
    //loadScene("/maps/untitled.xml");
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

  //Move the player
  player->velocity.x = 0;
  player->velocity.z = 0;

  player->velocity.y -= GRAVITY;

  float rotation = Math::toRadians(player->rotation.y);
  if(player->isAlive()){
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
      player->velocity.y = 0.3f;
    }
  }
  Vector3f pos = Vector3f::add(player->position, player->velocity);

  //Y collision
  BoxCollider bboxY(Vector3f(player->position.x, pos.y, player->position.z), player->scale);
  if (!collidesWithWalls(bboxY) || scene->bluePortal.inPortal(bboxY) || scene->orangePortal.inPortal(bboxY)) {
    player->position.y = pos.y;
  } else {
    if(player->velocity.y < 0) {
      player->grounded = true;
    }
    player->velocity.y = 0;
  }

  //X collision
  BoxCollider bboxX(Vector3f(pos.x, player->position.y, player->position.z), player->scale);
  if (!collidesWithWalls(bboxX) || scene->bluePortal.inPortal(bboxX) || scene->orangePortal.inPortal(bboxX)) {
    player->position.x = pos.x;
  } else {
    player->velocity.x = 0;
  }

  //Z collision
  BoxCollider bboxZ(Vector3f(player->position.x, player->position.y, pos.z), player->scale);
  if (!collidesWithWalls(bboxZ) || scene->bluePortal.inPortal(bboxZ) || scene->orangePortal.inPortal(bboxZ)) {
    player->position.z = pos.z;
  } else {
    player->velocity.z = 0;
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

  //Check if the player is walking through a portal
  if(scene->bluePortal.open && scene->orangePortal.open) {
    if(scene->bluePortal.throughPortal(player->position)) {
      player->position.set(scene->orangePortal.position);
      float rotation = scene->orangePortal.rotation.y - scene->bluePortal.rotation.y + 180;
      player->rotation.y += rotation;
    }
    if(scene->orangePortal.throughPortal(player->position)) {
       player->position.set(scene->bluePortal.position);
       float rotation = scene->bluePortal.rotation.y - scene->orangePortal.rotation.y + 180;
       player->rotation.y += rotation;
    }
  }

  //Parent camera to player
  scene->camera.position.set(scene->player.position);
  scene->camera.position.y += scene->player.scale.y/2;
  scene->camera.rotation.set(scene->player.rotation);

  //Check if the end of the level has been reached
  float distToEnd = Vector3f::sub(scene->end.position, scene->player.position).length();
  if(distToEnd < 1) {
    //FIXME Load the next scene
    loadScene("/maps/n2.xml");
  }
}

bool World::collidesWithWalls(BoxCollider collider) {
  for (unsigned int i = 0; i < scene->walls.size(); i++) {
    Entity wall = scene->walls[i];
    BoxCollider wallCollider(wall.position, wall.scale);

    if(collider.collidesWith(wallCollider)) {
      return true;
    }
  }
  return false;
}

void World::shootPortal(int button) {
  //Shooting
  Vector3f cameraDir(cos(Math::toRadians(scene->camera.rotation.x)) * -sin(Math::toRadians(scene->camera.rotation.y)),
                     sin(Math::toRadians(scene->camera.rotation.x)),
                     -cos(Math::toRadians(scene->camera.rotation.x)) * cos(Math::toRadians(scene->camera.rotation.y)));

  //Find the closest intersection
  Entity closestWall;
  float intersection = INT_MAX;
  for (unsigned int i = 0; i < scene->walls.size(); i++) {
    Entity wall = scene->walls[i];
    Ray bullet = Ray(scene->camera.position, cameraDir);
    float tNear, tFar;
    if (bullet.collides(wall, &tNear, &tFar)) {
      if(wall.texture.handle == TextureLoader::getTexture("wall.png").handle) {
        if (tNear < intersection) {
          closestWall = wall;
          intersection = tNear;
        }
      }
    }
  }

  Vector3f ipos = Vector3f::add(scene->camera.position, Vector3f::scale(cameraDir, intersection));
  //Determine on what side the portal is
  //Side 0: -x, Side 1: x, Side 2: -z, Side 3: z, Side 4: -y, Side 5: y
  float dist = 1000000;
  int side = 0;
  float distances[6] = {(float) fabs(ipos.x - (closestWall.position.x - closestWall.scale.x / 2)),
                        (float) fabs(ipos.x - (closestWall.position.x + closestWall.scale.x / 2)),
                        (float) fabs(ipos.z - (closestWall.position.z - closestWall.scale.z / 2)),
                        (float) fabs(ipos.z - (closestWall.position.z + closestWall.scale.z / 2)),
                        (float) fabs(ipos.y - (closestWall.position.y - closestWall.scale.y / 2)),
                        (float) fabs(ipos.y - (closestWall.position.y + closestWall.scale.y / 2))};

  for (int i = 0; i < 6; i++) {
    if (distances[i] < dist) {
      side = i;
      dist = distances[i];
    }
  }

  Light light;
  light.position.set(ipos);
  light.constantAtt = 0;
  light.linearAtt = 1;
  light.quadraticAtt = 0;

  Portal portal;
  portal.position.set(ipos.x, ipos.y, ipos.z);
  portal.scale.set(1, 2, 1);
  if (side == 0) {
    portal.rotation.y = 90;
    portal.position.x -= 0.01;
    light.position.x -= 0.5;
  }
  if (side == 1) {
    portal.rotation.y = -90;
    portal.position.x += 0.01;
    light.position.x += 0.5;
  }
  if (side == 2) {
    portal.rotation.y = 0;
    portal.position.z -= 0.01;
    light.position.z -= 0.5;
  }
  if (side == 3) {
    portal.rotation.y = 180;
    portal.position.z += 0.01;
    light.position.z += 0.5;
  }
  if (side == 4) {
    portal.rotation.x = -90;
    portal.position.y -= 0.01;
    light.position.y -= 0.5;
  }
  if (side == 5) {
    portal.rotation.x = 90;
    portal.position.y += 0.01;
    light.position.y += 0.5;
  }

  portal.open = true;
  portal.mesh = MeshLoader::getMesh("Plane.obj");

  if (button == 1) {
    portal.texture = TextureLoader::getTexture("blueportal.png");
    light.color.set(Portal::BLUE_COLOR);
    portal.light = light;
    scene->bluePortal = portal;
  } else {
    portal.texture = TextureLoader::getTexture("orangeportal.png");
    light.color.set(Portal::ORANGE_COLOR);
    portal.light = light;
    scene->orangePortal = portal;
  }
}

void World::render() {
  renderer->render(scene);
}

} /* namespace glPortal */
