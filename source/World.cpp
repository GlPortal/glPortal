#include "World.hpp"

#include <SDL2/SDL_mouse.h>
#include <cmath>
#include <cstdlib>
#include <string>
#include <vector>
#include <stdio.h>
#include <iostream>

#include "engine/BoxCollider.hpp"
#include "engine/Camera.hpp"
#include "engine/Light.hpp"
#include "engine/MapLoader.hpp"
#include "engine/Mesh.hpp"
#include "engine/MeshLoader.hpp"
#include "engine/Renderer.hpp"
#include "engine/Texture.hpp"
#include "engine/TextureLoader.hpp"
#include "engine/util/Vector2f.hpp"
#include "engine/util/Vector3f.hpp"
#include "engine/util/Math.hpp"
#include "engine/XmlMapLoader.hpp"
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

  loadScene("data/maps/n1.map");
  //scene = XmlMapLoader::getScene(std::string("/maps/untitled.xml"));
  //scene = XmlMapLoader::getScene(std::string("/maps/first.xml"));
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

  float rotation = player->rotation.y * Math::PI_RND / 180;

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
  if (Input::isKeyDown(' ')) {
    player->velocity.y = 0.2;
  }

  Vector3f pos = Vector3f::add(player->position, player->velocity);

  //Y collision
  BoxCollider bboxY(Vector3f(player->position.x, pos.y, player->position.z), player->scale);
  if (!collidesWithWalls(bboxY) || scene->bluePortal.inPortal(bboxY) || scene->orangePortal.inPortal(bboxY)) {
    player->position.y = pos.y;
  } else {
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
    BoxCollider bboxTrigger(trigger.position, trigger.scale);

    //Y collision
    BoxCollider bboxTriggerY(Vector3f(player->position.x, pos.y, player->position.z), player->scale);
    if (bboxTriggerY.collidesWith(bboxTrigger)) {
      std::cout << "trigger touched\n";
    }
    //X collision
    BoxCollider bboxTriggerX(Vector3f(pos.x, player->position.y, player->position.z), player->scale);
    if (bboxTriggerX.collidesWith(bboxTrigger)) {
      std::cout << "trigger touched\n";
    }
    //Z collision
    BoxCollider bboxTriggerZ(Vector3f(player->position.x, player->position.y, pos.z), player->scale);
    if (bboxTriggerZ.collidesWith(bboxTrigger)) {
      std::cout << "trigger touched\n";
    }
  }

  //Check if the player is walking through a portal
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

  //Parent camera to player
  scene->camera.position.set(scene->player.position);
  scene->camera.position.y += scene->player.scale.y/2;
  scene->camera.rotation.set(scene->player.rotation);

  //Check if the end of the level has been reached
  float distToEnd = Vector3f::sub(scene->end.position, scene->player.position).length();
  if(distToEnd < 1) {
    //FIXME Load the next scene
    loadScene("data/maps/n1.map");
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
  Vector3f cameraDir(cos(Math::DEG_TO_RAD(scene->camera.rotation.x)) * -sin(Math::DEG_TO_RAD(scene->camera.rotation.y)),
                     sin(Math::DEG_TO_RAD(scene->camera.rotation.x)),
                     -cos(Math::DEG_TO_RAD(scene->camera.rotation.x)) * cos(Math::DEG_TO_RAD(scene->camera.rotation.y)));

  //Find the closest intersection
  Entity closestWall;
  float intersection = 5000000;
  for (unsigned int i = 0; i < scene->walls.size(); i++) {
    Entity wall = scene->walls[i];
    float tNear, tFar;
    if (collides(scene->camera.position, cameraDir, wall, &tNear, &tFar)) {
      if (tNear < intersection) {
        closestWall = wall;
        intersection = tNear;
      }
    }
  }

  Vector3f ipos = Vector3f::add(scene->camera.position, Vector3f::scale(cameraDir, intersection));
  //Determine on what side the portal is
  //Side 0: -x, Side 1: x, Side 2: -z, Side 3: z, Side 4: -y, Side 5: y
  float dist = 1000000;
  int side = 0;
  float distances[6];
  distances[0] = fabs(ipos.x - (closestWall.position.x - closestWall.scale.x / 2));
  distances[1] = fabs(ipos.x - (closestWall.position.x + closestWall.scale.x / 2));
  distances[2] = fabs(ipos.z - (closestWall.position.z - closestWall.scale.z / 2));
  distances[3] = fabs(ipos.z - (closestWall.position.z + closestWall.scale.z / 2));
  distances[4] = fabs(ipos.y - (closestWall.position.y - closestWall.scale.y / 2));
  distances[5] = fabs(ipos.y - (closestWall.position.y + closestWall.scale.y / 2));

  for (int i = 0; i < 6; i++) {
    if (distances[i] < dist) {
      side = i;
      dist = distances[i];
    }
  }

  //Temp spawn a light
  Light light;
  light.position.set(ipos);
  light.linearAtt = 0.8;

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
  portal.mesh = MeshLoader::getMesh("data/meshes/Portal.obj");
  if (button == 1) {
    portal.texture = TextureLoader::getTexture("data/textures/blueportal.png");
    scene->bluePortal = portal;
    light.color.set(0.33, 0.57, 1);
  } else {
    portal.texture = TextureLoader::getTexture("data/textures/orangeportal.png");
    scene->orangePortal = portal;
    light.color.set(1, 0.76, 0.33);
  }
  scene->lights.push_back(light);
}

bool World::collides(Vector3f ro, Vector3f rd, Entity e, float* tNear, float* tFar) {
  float min[3] = {e.position.x - e.scale.x / 2,
                  e.position.y - e.scale.y / 2,
                  e.position.z - e.scale.z / 2};
  float max[3] = {e.position.x + e.scale.x / 2,
                  e.position.y + e.scale.y / 2,
                  e.position.z + e.scale.z / 2};

  float origin[3] = { ro.x, ro.y, ro.z };
  float dir[3] = { rd.x, rd.y, rd.z };

  *tNear = -5000000.0f;
  *tFar = 50000000.0f;

  for (int i = 0; i < 3; i++) {
    //Parallel lines
    if (dir[i] == 0) {
      if (origin[i] < min[i] || origin[i] > max[i]) {
        return false;
      }
    } else {
      //Slab intersection
      float t1 = (min[i] - origin[i]) / dir[i];
      float t2 = (max[i] - origin[i]) / dir[i];
      //If t1 is bigger than t2 switch them
      if (t1 > t2) {
        float temp = t1;
        t1 = t2;
        t2 = temp;
      }
      if (t1 > *tNear) {
        *tNear = t1;
      }
      if (t2 < *tFar) {
        *tFar = t2;
      }

      //End
      if (*tNear > *tFar) {
        return false;
      }
      if (*tFar < 0) {
        return false;
      }
    }
  }
  return true;
}

void World::render() {
  renderer->render(scene);
}

} /* namespace glPortal */
