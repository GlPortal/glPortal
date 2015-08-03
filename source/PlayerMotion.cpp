#include "PlayerMotion.hpp"

#include <engine/core/math/Math.hpp>
#include <engine/core/math/Vector2f.hpp>
#include <engine/env/Environment.hpp>
#include <engine/component/Health.hpp>
#include <engine/component/SoundSource.hpp>
#include <engine/component/RigidBody.hpp>
#include <Input.hpp>
#include <SDL2/SDL_mouse.h>
#include <SDL2/SDL_keyboard.h>
#include <World.hpp>
#include <cmath>

namespace glPortal {

static const float FRICTION = 0.001;
static const float MIN_SPEED_ON_AXIS = FRICTION;

// Movement
void PlayerMotion::mouseLook() {
  int mousedx, mousedy;
  SDL_GetRelativeMouseState(&mousedx, &mousedy);

  // Apply mouse movement to view
  //Vector3f &rotation = entity.getComponent<Transform>().rotation;
  rotation.x -= rad(mousedy * Config::getSensitivity());
  rotation.y -= rad(mousedx * Config::getSensitivity());

  // Restrict rotation in horizontal axis
  rotation.x = Math::clamp(rotation.x, rad(-90.0f), rad(90.0f));
}

void PlayerMotion::move(float dtime) {
  bool movingFwd     = Input::isKeyDown(SDL_SCANCODE_W) or Input::isKeyDown(SDL_SCANCODE_UP),
       movingBack    = Input::isKeyDown(SDL_SCANCODE_S) or Input::isKeyDown(SDL_SCANCODE_DOWN),
       strafingLeft  = Input::isKeyDown(SDL_SCANCODE_A) or Input::isKeyDown(SDL_SCANCODE_LEFT),
       strafingRight = Input::isKeyDown(SDL_SCANCODE_D) or Input::isKeyDown(SDL_SCANCODE_RIGHT);
       float rot = rotation.y;
  if (movingFwd) {
    Vector3f force; force.x -= sin(rot);
      force.z -= cos(rot);
    entity.getComponent<RigidBody>().body->applyCentralForce(force*4);
  }
#if 0
  if (velocity.x <= MIN_SPEED_ON_AXIS and velocity.x >= -MIN_SPEED_ON_AXIS) {
    velocity.x = 0;
  } else {
    velocity.x *= std::pow(FRICTION, dtime);
  }
  if (velocity.z <= MIN_SPEED_ON_AXIS and velocity.z >= -MIN_SPEED_ON_AXIS) {
    velocity.z = 0;
  } else {
    velocity.z *= std::pow(FRICTION, dtime);
  }
  if (not grounded) {
    if (flying) {
      if (velocity.y <= MIN_SPEED_ON_AXIS and velocity.y >= -MIN_SPEED_ON_AXIS) {
        velocity.y = 0;
      } else {
        velocity.y *= std::pow(FRICTION, dtime);
      }
    } else {
      velocity.y -= World::gravity * dtime;
    }
  }

  float rot = rotation.y;
  if (entity.getComponent<Health>().isAlive()) {
    Vector3f tmpVel;
    double yMult = flying ? cos(rotation.x) : 1;
    if (movingFwd) {
      tmpVel.x -= sin(rot) * yMult;
      tmpVel.z -= cos(rot) * yMult;
      if (flying) {
        tmpVel.y += sin(rotation.x);
      }
    }
    if (movingBack) {
      tmpVel.x += sin(rot) * yMult;
      tmpVel.z += cos(rot) * yMult;
      if (flying) {
        tmpVel.y -= sin(rotation.x);
      }
    }
    if (strafingLeft) {
      tmpVel.x += -cos(rot);
      tmpVel.z += sin(rot);
    }
    if (strafingRight) {
      tmpVel.x += cos(rot);
      tmpVel.z += -sin(rot);
    }
    float tmpVelLen = tmpVel.length();
    if (tmpVelLen > 0) {
      tmpVel.normalise();
    }
    tmpVel *= speed;
    if (movingFwd or movingBack or strafingLeft or strafingRight) {
      velocity.x = tmpVel.x;
      velocity.z = tmpVel.z;
      if (flying) {
        velocity.y = tmpVel.y;
      }
    }


    if ((Input::isKeyDown(SDL_SCANCODE_SPACE) || Input::isKeyDown(SDL_SCANCODE_BACKSPACE)) && grounded) {
      std::uniform_int_distribution<> dis(0, PLAYER_JUMP_SOUND.size()-1);
      entity.getComponent<SoundSource>().playSound(
        Environment::getDataDir() + PLAYER_JUMP_SOUND[dis(generator)]);
      grounded = false;
      velocity.y = JUMP_SPEED;
    }

    if (grounded) {
      stepCounter += abs(velocity.x);
      stepCounter += abs(velocity.z);

      if(stepCounter>=2.5f) {
        std::uniform_int_distribution<> dis(0, PLAYER_FOOT_SOUND.size()-1);
        entity.getComponent<SoundSource>().playSound(
          Environment::getDataDir() + PLAYER_FOOT_SOUND[dis(generator)]);
        stepCounter-=2.5f;
      }
    }
  }
#endif
}

} /* namespace glPortal */
