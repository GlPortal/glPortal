#include <engine/core/math/Math.hpp>
#include <engine/core/math/Vector2f.hpp>
#include <engine/SoundManager.hpp>
#include <engine/env/Environment.hpp>
#include <Input.hpp>
#include <Player.hpp>
#include <SDL2/SDL_mouse.h>
#include <SDL2/SDL_keyboard.h>
#include <World.hpp>
#include <cmath>

namespace glPortal {

// Movement
void Player::mouseLook() {
  int mousedx, mousedy;
  SDL_GetRelativeMouseState(&mousedx, &mousedy);

  // Apply mouse movement to view
  rotation.x -= mousedy * Config::getSensitivity();
  rotation.y -= mousedx * Config::getSensitivity();

  // Restrict rotation in horizontal axis
  rotation.x = Math::clamp(rotation.x, -90, 90);
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
    Vector3f tmp_velocity;
    tmp_velocity.x = tmp_velocity.z = 0;

    if (Input::isKeyDown(SDL_SCANCODE_W) or Input::isKeyDown(SDL_SCANCODE_UP)) {
      tmp_velocity.x += -sin(rot) * speed;
      tmp_velocity.z += -cos(rot) * speed;
    }
    if (Input::isKeyDown(SDL_SCANCODE_S) or Input::isKeyDown(SDL_SCANCODE_DOWN)) {
      tmp_velocity.x += sin(rot) * speed;
      tmp_velocity.z += cos(rot) * speed;
    }
    if (Input::isKeyDown(SDL_SCANCODE_A) or Input::isKeyDown(SDL_SCANCODE_LEFT)) {
      tmp_velocity.x += -cos(rot) * speed;
      tmp_velocity.z += sin(rot) * speed;
    }
    if (Input::isKeyDown(SDL_SCANCODE_D) or Input::isKeyDown(SDL_SCANCODE_RIGHT)) {
      tmp_velocity.x += cos(rot) * speed;
      tmp_velocity.z += -sin(rot) * speed;
    }
    if (Input::isKeyDown(SDL_SCANCODE_W) or Input::isKeyDown(SDL_SCANCODE_UP) or
        Input::isKeyDown(SDL_SCANCODE_S) or Input::isKeyDown(SDL_SCANCODE_DOWN) or
        Input::isKeyDown(SDL_SCANCODE_A) or Input::isKeyDown(SDL_SCANCODE_LEFT) or
        Input::isKeyDown(SDL_SCANCODE_D) or Input::isKeyDown(SDL_SCANCODE_RIGHT))
    {
      velocity.x = tmp_velocity.x;
      velocity.z = tmp_velocity.z;
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
