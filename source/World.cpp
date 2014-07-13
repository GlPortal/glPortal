#include "World.hpp"

#include <SDL2/SDL_mouse.h>
#include <cmath>
#include <cstdlib>
#include <string>
#include <vector>

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
#include "Input.hpp"
#include "Player.hpp"
#include "Portal.hpp"
#include "Scene.hpp"

namespace glPortal {

const float PI = 3.14159265358979323846;
const Vector2f sensitivity(0.5, 0.5);
const float SPEED = 0.5;
const float GRAVITY = 0.05;

void World::create() {
  renderer = new Renderer();
  scene = MapLoader::getScene(std::string("data/maps/n1.map"));
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
  if (player->rotation.x < -90)
    player->rotation.x = -90;
  if (player->rotation.x > 90)
    player->rotation.x = 90;

  //Move the player
  player->velocity.x = 0;
  player->velocity.z = 0;

  player->velocity.y -= GRAVITY;

  float rotation = player->rotation.y * PI / 180;

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

  //Portal teleportation
  usePortal(scene->bluePortal, scene->orangePortal);
  usePortal(scene->orangePortal, scene->bluePortal);

  //Collision
  for (unsigned int i = 0; i < scene->walls.size(); i++) {
    Entity wall = scene->walls[i];
    //Y collision
    if (player->position.x + player->scale.x / 2 > wall.position.x - wall.scale.x / 2 &&
        player->position.x - player->scale.x / 2 < wall.position.x + wall.scale.x / 2 &&
        pos.y + player->scale.y / 2 > wall.position.y - wall.scale.y / 2 &&
        pos.y - player->scale.y / 2 < wall.position.y + wall.scale.y / 2 &&
        player->position.z + player->scale.z / 2 > wall.position.z - wall.scale.z / 2 &&
        player->position.z - player->scale.z / 2 < wall.position.z + wall.scale.z / 2) {
      player->velocity.y = 0;
    }
    //X collision
    if (pos.x + player->scale.x / 2 > wall.position.x - wall.scale.x / 2 &&
        pos.x - player->scale.x / 2 < wall.position.x + wall.scale.x / 2 &&
        player->position.y + player->scale.y / 2 > wall.position.y - wall.scale.y / 2 &&
        player->position.y - player->scale.y / 2 < wall.position.y + wall.scale.y / 2 &&
        player->position.z + player->scale.z / 2 > wall.position.z - wall.scale.z / 2 &&
        player->position.z - player->scale.z / 2 < wall.position.z + wall.scale.z / 2) {
      player->velocity.x = 0;
    }
    //Z collision
    if (player->position.x + player->scale.x / 2 > wall.position.x - wall.scale.x / 2 &&
        player->position.x - player->scale.x / 2 < wall.position.x + wall.scale.x / 2 &&
        player->position.y + player->scale.y / 2 > wall.position.y - wall.scale.y / 2 &&
        player->position.y - player->scale.y / 2 < wall.position.y + wall.scale.y / 2 &&
        pos.z + player->scale.z / 2 > wall.position.z - wall.scale.z / 2 &&
        pos.z - player->scale.z / 2 < wall.position.z + wall.scale.z / 2) {
      player->velocity.z = 0;
    }
  }
  player->position.add(player->velocity);

  scene->camera.position.set(scene->player.position);
  scene->camera.rotation.set(scene->player.rotation);
}

void World::usePortal(Entity srcPortal, Entity destPortal) {
  Player* player = &scene->player;
  if (srcPortal.rotation.y == 90 || srcPortal.rotation.y == -90) {
    if (srcPortal.position.x > player->position.x - player->scale.x / 2
        && srcPortal.position.x < player->position.x + player->scale.x / 2
        && player->position.y + player->scale.y / 2 > srcPortal.position.y - 1
        && player->position.y - player->scale.y / 2 < srcPortal.position.y + 1
        && player->position.z + player->scale.z / 2 > srcPortal.position.z - 0.5
        && player->position.z - player->scale.z / 2 < srcPortal.position.z + 0.5) {
      player->position.set(destPortal.position);
      if (destPortal.rotation.y == -90) {
        player->position.x -= player->scale.x;
      }
      if (destPortal.rotation.y == 90) {
        player->position.x += player->scale.x;
      }
    }
  }
  if ((srcPortal.rotation.y == 0 && srcPortal.rotation.x == 0) || (srcPortal.rotation.y == 180 && srcPortal.rotation.x == 0)) {
    if (player->position.x + player->scale.x / 2 > srcPortal.position.x - 0.5
        && player->position.x - player->scale.x / 2 < srcPortal.position.x + 0.5
        && player->position.y + player->scale.y / 2 > srcPortal.position.y - 1
        && player->position.y - player->scale.y / 2 < srcPortal.position.y + 1
        && srcPortal.position.z > player->position.z - player->scale.z / 2
        && srcPortal.position.z < player->position.z + player->scale.z / 2) {
      player->position.set(destPortal.position);
      if (destPortal.rotation.y == 0) {
        player->position.z += player->scale.z;
      }
      if (destPortal.rotation.y == 180) {
        player->position.z -= player->scale.z;
      }
    }
  }
  if (srcPortal.rotation.x == 90 || srcPortal.rotation.x == -90) {
    if (player->position.x + player->scale.x / 2 > srcPortal.position.x - 0.5
        && player->position.x - player->scale.x / 2 < srcPortal.position.x + 0.5
        && srcPortal.position.y > player->position.y - player->scale.y / 2
        && srcPortal.position.y < player->position.y + player->scale.y / 2
        && player->position.z + player->scale.z / 2 > srcPortal.position.z - 1
        && player->position.z - player->scale.z / 2 < srcPortal.position.z + 1) {
      player->position.set(destPortal.position);
      if (destPortal.rotation.x == -90) {
        player->position.y += player->scale.y;
      }
      if (destPortal.rotation.x == 90) {
        player->position.y -= player->scale.y;
      }
    }
  }
}

void World::shootPortal(int button) {
  //Shooting
  Vector3f cameraDir(
      cos(scene->camera.rotation.x * PI / 180)
          * -sin(scene->camera.rotation.y * PI / 180),
      sin(scene->camera.rotation.x * PI / 180),
      -cos(scene->camera.rotation.x * PI / 180)
          * cos(scene->camera.rotation.y * PI / 180));

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
  distances[0] = abs(ipos.x - (closestWall.position.x - closestWall.scale.x / 2));
  distances[1] = abs(ipos.x - (closestWall.position.x + closestWall.scale.x / 2));
  distances[2] = abs(ipos.z - (closestWall.position.z - closestWall.scale.z / 2));
  distances[3] = abs(ipos.z - (closestWall.position.z + closestWall.scale.z / 2));
  distances[4] = abs(ipos.y - (closestWall.position.y - closestWall.scale.y / 2));
  distances[5] = abs(ipos.y - (closestWall.position.y + closestWall.scale.y / 2));

  for (int i = 0; i < 6; i++) {
    if (distances[i] < dist) {
      side = i;
      dist = distances[i];
    }
  }

  //Temp spawn a light
  Light light;
  light.position.set(ipos);
  light.constantAtt = 0.5;
  light.linearAtt = 0.4;
  light.quadraticAtt = 0.2;

  Portal portal;
  portal.position.set(ipos.x, ipos.y, ipos.z);
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
