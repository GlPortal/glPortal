#include <engine/core/math/Math.hpp>
#include <engine/core/math/Vector2f.hpp>
#include <engine/SoundManager.hpp>
#include <engine/env/Environment.hpp>
#include <Input.hpp>
#include <Player.hpp>
#include <SDL2/SDL_mouse.h>
#include <SDL.h>
#include <World.hpp>
#include <cmath>

namespace glPortal {

// Movement
void Player::mouseLook() {
  int mousedx, mousedy;
  SDL_GetRelativeMouseState(&mousedx, &mousedy);

  // Apply mouse movement to view
  rotation.x -= mousedy * sensitivity.x;
  rotation.y -= mousedx * sensitivity.y;

  // Restrict rotation in horizontal axis
  if (rotation.x < -90) {
    rotation.x = -90;
  }
  if (rotation.x > 90) {
    rotation.x = 90;
  }
}

void Player::move() {
  if (velocity.x >= FRICTION) {
    velocity.x -= FRICTION;
  }
  if (velocity.x <= -FRICTION) {
    velocity.x += FRICTION;
  }
  if (velocity.z >= FRICTION) {
    velocity.z -= FRICTION;
  }
  if (velocity.z <= -FRICTION) {
    velocity.z += FRICTION;
  }
  if (velocity.x < FRICTION && velocity.x > -FRICTION) {velocity.x = 0;}
  if (velocity.z < FRICTION && velocity.z > -FRICTION) {velocity.z = 0;}
  velocity.y -= World::gravity;

  float rot = Math::toRadians(rotation.y);
  if (isAlive()){
    if (Input::isKeyDown(SDL_SCANCODE_W) or Input::isKeyDown(SDL_SCANCODE_UP)) {
      velocity.x = -sin(rot) * speed;
      velocity.z = -cos(rot) * speed;
    }
    if (Input::isKeyDown(SDL_SCANCODE_S) or Input::isKeyDown(SDL_SCANCODE_DOWN)) {
      velocity.x = sin(rot) * speed;
      velocity.z = cos(rot) * speed;
    }
    if (Input::isKeyDown(SDL_SCANCODE_A) or Input::isKeyDown(SDL_SCANCODE_LEFT)) {
      velocity.x = -cos(rot) * speed;
      velocity.z = sin(rot) * speed;
    }
    if (Input::isKeyDown(SDL_SCANCODE_D) or Input::isKeyDown(SDL_SCANCODE_RIGHT)) {
      velocity.x = cos(rot) * speed;
      velocity.z = -sin(rot) * speed;
    }
    if (Input::isKeyDown(SDL_SCANCODE_SPACE) && grounded) {
	  std::uniform_int_distribution<> dis(0, PLAYER_JUMP_SOUND.size()-1);   
	  SoundManager::PlaySound(Environment::getDataDir() + PLAYER_JUMP_SOUND[dis(generator)],this,SoundManager::PRIMARY);
      grounded = false;
      velocity.y = JUMP_SPEED;
    }

    if(grounded)
    {
      stepCounter += abs(velocity.x);
      stepCounter += abs(velocity.z);
      if(stepCounter>=2.5f)
      {
        std::uniform_int_distribution<> dis(0, PLAYER_FOOT_SOUND.size()-1);   
        SoundManager::PlaySound(Environment::getDataDir() + PLAYER_FOOT_SOUND[dis(generator)],this,SoundManager::SECONDARY);
        stepCounter-=2.5f;
      }
    }
  }
}

// Health
int Player::getHealth() {
  return health;
}

bool Player::isAlive() {
  if (health > 0) {
    return true;
  }
  return false;
}

void Player::heal(int amount) {
  health += amount;
  if (health > MAX_HEALTH) {
    health = MAX_HEALTH;
  }
}

void Player::harm(int amount) {
  health -= amount;
  if (health < MIN_HEALTH) {
    health = MIN_HEALTH;
  }
}

void Player::kill() {
  health = 0;
}

void Player::setPlayingSound(bool state)
{
  playingSound = state;
}

bool Player::getPlayingSound()
{
  return playingSound;
}

} /* namespace glPortal */
