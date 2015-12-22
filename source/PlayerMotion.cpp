#include "PlayerMotion.hpp"

#include <engine/core/math/Math.hpp>
#include <engine/core/math/Vector2f.hpp>
#include <engine/env/Environment.hpp>
#include <engine/component/Health.hpp>
#include <engine/component/SoundSource.hpp>
#include <engine/component/RigidBody.hpp>
#include <engine/component/Player.hpp>
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
  headAngle.attitude -= rad(mousedy * Environment::getConfig().getSensitivity());
  headAngle.heading  -= rad(mousedx * Environment::getConfig().getSensitivity());
  headAngle.tilt *= 0.8;

  // Restrict rotation in horizontal axis
  headAngle.attitude = Math::clamp(headAngle.attitude, rad(-89.99f), rad(89.99f));
}

Quaternion PlayerMotion::getBaseHeadOrientation() const {
  return Quaternion().fromAero(headAngle);
}

Quaternion PlayerMotion::getHeadOrientation() const {
  return Quaternion().fromAero(headAngle);
}

void PlayerMotion::move(float dtime) {
  bool movingFwd     = Input::isKeyDown(SDL_SCANCODE_W) or Input::isKeyDown(SDL_SCANCODE_UP),
       movingBack    = Input::isKeyDown(SDL_SCANCODE_S) or Input::isKeyDown(SDL_SCANCODE_DOWN),
       strafingLeft  = Input::isKeyDown(SDL_SCANCODE_A) or Input::isKeyDown(SDL_SCANCODE_LEFT),
       strafingRight = Input::isKeyDown(SDL_SCANCODE_D) or Input::isKeyDown(SDL_SCANCODE_RIGHT),
       jumping       = Input::isKeyDown(SDL_SCANCODE_SPACE) or Input::isKeyDown(SDL_SCANCODE_BACKSPACE);
  float rot = headAngle.heading;
  Vector3f movement;
  KinematicCharacterController &ctrl = *entity.getComponent<Player>().controller;
  Transform &plrTform = entity.getComponent<Transform>();
  plrTform.privSetPosition(entity.getComponent<Player>().obj->getWorldTransform().getOrigin());

  if (jumping and ctrl.canJump()) {
    std::uniform_int_distribution<> dis(0, PLAYER_JUMP_SOUND.size()-1);
      entity.getComponent<SoundSource>().playSound(
        Environment::getDataDir() + PLAYER_JUMP_SOUND[dis(generator)]);
    ctrl.jump();
  }
  if (movingFwd) {
    movement.x += -sin(rot);
    movement.z += -cos(rot);
  }
  if (movingBack) {
    movement.x += sin(rot);
    movement.z += cos(rot);
  }
  if (strafingLeft) {
    movement.x += -cos(rot);
    movement.z += sin(rot);
  }
  if (strafingRight) {
    movement.x += cos(rot);
    movement.z += -sin(rot);
  }

  ctrl.setWalkDirection(movement*0.1);

  if (ctrl.onGround()) {
    stepCounter += abs(velocity.x);
    stepCounter += abs(velocity.z);

    if (stepCounter >= 2.5f) {
      std::uniform_int_distribution<> dis(0, PLAYER_FOOT_SOUND.size()-1);
      entity.getComponent<SoundSource>().playSound(
        Environment::getDataDir() + PLAYER_FOOT_SOUND[dis(generator)]);
      stepCounter -= 2.5f;
    }
  }
#if 0
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
#endif
}

} /* namespace glPortal */
