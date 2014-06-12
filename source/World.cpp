#include "World.hpp"
#include <SDL2/SDL.h>
#include "Input.hpp"
#include "engine/MapLoader.hpp"
#include "engine/util/Vector2f.hpp"
#include <iostream>
#include <cmath>

namespace glPortal {

const float PI = 3.14159265358979323846;
const Vector2f sensitivity(0.5, 0.5);
const float SPEED = 0.5;

void World::create() {
  renderer = new Renderer();
  scene = MapLoader::getScene(std::string("data/n1.map"));
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
  if(player->rotation.x < -90) player->rotation.x = -90;
  if(player->rotation.x >  90) player->rotation.x =  90;

  //Move the player
  player->velocity.x = 0;
  player->velocity.z = 0;

  player->velocity.y -= 0.05;

  float rotation = player->rotation.y * PI/180;

  if(Input::isKeyDown('w')) {
    player->velocity.x = -sin(rotation) * SPEED;
    player->velocity.z = -cos(rotation) * SPEED;
  }
  if(Input::isKeyDown('s')) {
    player->velocity.x = sin(rotation) * SPEED;
    player->velocity.z = cos(rotation) * SPEED;
  }
  if(Input::isKeyDown('a')) {
    player->velocity.x = -cos(rotation) * SPEED;
    player->velocity.z = sin(rotation) * SPEED;
  }
  if(Input::isKeyDown('d')) {
    player->velocity.x = cos(rotation) * SPEED;
    player->velocity.z = -sin(rotation) * SPEED;
  }
  if(Input::isKeyDown(' ')) {
    player->velocity.y = 0.2;
  }

  Vector3f pos = Vector3f::add(player->position, player->velocity);

  //Collision
  for(unsigned int i = 0; i < scene->walls.size(); i++) {
    Entity wall = scene->walls[i];
    //Y collision
    if(pos.y - 1 < wall.position.y + wall.scale.y/2 && pos.y + 1 > wall.position.y -wall.scale.y/2 &&
       pos.x > wall.position.x - wall.scale.x/2 && pos.x < wall.position.x + wall.scale.x/2 &&
       pos.z > wall.position.z - wall.scale.z/2 && pos.z < wall.position.z + wall.scale.z/2) {
      std::cout << player->velocity.y << std::endl;
      player->velocity.y = 0;
    }
    //X collision
    if(pos.x + 0.5 > wall.position.x - wall.scale.x/2 && pos.x - 0.5 < wall.position.x + wall.scale.x/2 &&
       pos.y > wall.position.y - wall.scale.y/2 && pos.y < wall.position.y + wall.scale.y/2 &&
       pos.z > wall.position.z - wall.scale.z/2 && pos.z < wall.position.z + wall.scale.z/2) {
      player->velocity.x = 0;
    }
    //Z collision
    if(pos.z + 0.5 > wall.position.z - wall.scale.z/2 && pos.z - 0.5 < wall.position.z + wall.scale.z/2 &&
       pos.y > wall.position.y - wall.scale.y/2 && pos.y < wall.position.y + wall.scale.y/2 &&
       pos.x > wall.position.x - wall.scale.x/2 && pos.x < wall.position.x + wall.scale.x/2) {
      player->velocity.z = 0;
    }
  }
  player->position.add(player->velocity);
}

void World::render() {
  renderer->render(scene);
}

} /* namespace glPortal */
