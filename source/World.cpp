#include "World.hpp"
#include <SDL2/SDL.h>
#include "Input.hpp"
#include "engine/MapLoader.hpp"
#include <iostream>
#include <cmath>

#define PI 3.14159265358979323846

namespace glPortal {

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
  player->rotation.x -= mousedy;
  player->rotation.y -= mousedx;

  // Restrict rotation in horizontal axis
  if(player->rotation.x < -90) player->rotation.x = -90;
  if(player->rotation.x >  90) player->rotation.x =  90;

  //Move the player
  player->velocity.x = 0;
  player->velocity.z = 0;

  float rotation = player->rotation.y * PI/180;

  if(Input::isKeyDown('w')) {
    player->velocity.x = -sin(rotation);
    player->velocity.z = -cos(rotation);
  }
  if(Input::isKeyDown('s')) {
    player->velocity.x = sin(rotation);
    player->velocity.z = cos(rotation);
  }
  if(Input::isKeyDown('a')) {
    player->velocity.x = -cos(rotation);
    player->velocity.z = sin(rotation);
  }
  if(Input::isKeyDown('d')) {
    player->velocity.x = cos(rotation);
    player->velocity.z = -sin(rotation);
  }

  player->position.add(player->velocity);
}

void World::render() {
  renderer->render(scene);
}

} /* namespace glPortal */
