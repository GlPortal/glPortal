#include "PlayerSystem.hpp"

#include <SDL2/SDL_mouse.h>
#include <SDL2/SDL_keyboard.h>

#include <engine/core/math/Math.hpp>
#include <engine/env/Environment.hpp>
#include <Input.hpp>
#include <engine/component/Health.hpp>
#include <engine/component/SoundSource.hpp>
#include <engine/component/RigidBody.hpp>
#include <engine/component/Player.hpp>

namespace glPortal {

static const float RUNNING_SPEED = 0.1f;
static const float JUMP_SPEED = 0.15f;
static const float HURT_VELOCITY = 0.18f;

static const std::array<const std::string, 2> PLAYER_PANTING_SOUND =
{
  "/audio/sfx/character/fem_panting_1.ogg",
  "/audio/sfx/character/fem_panting_2.ogg"
};

static const std::array<const std::string, 2> PLAYER_JUMP_SOUND =
{
  "/audio/sfx/character/fem_jump_1.ogg",
  "/audio/sfx/character/fem_jump_2.ogg"
};

static const std::array<const std::string, 2> PLAYER_FALL_SOUND =
{
  "/audio/sfx/character/fem_fall_1.ogg",
  "/audio/sfx/character/fem_fall_2.ogg"
};

static const std::array<const std::string, 6> PLAYER_FOOT_SOUND =
{
  "/audio/sfx/character/fem_foot_1.ogg",
  "/audio/sfx/character/fem_foot_2.ogg",
  "/audio/sfx/character/fem_foot_3.ogg",
  "/audio/sfx/character/fem_foot_4.ogg",
  "/audio/sfx/character/fem_foot_5.ogg",
  "/audio/sfx/character/fem_foot_6.ogg"
};

void PlayerSystem::setScene(Scene *scene) {
  this->scene = scene;
}

PlayerSystem::PlayerSystem() :
  scene(nullptr) {
}

PlayerSystem::~PlayerSystem() {
}

void PlayerSystem::mouseLook(Entity &entity) {
  Player &plr = entity.getComponent<Player>();
  if (plr.frozen) {
    return;
  }
  int mousedx, mousedy;
  SDL_GetRelativeMouseState(&mousedx, &mousedy);

  // Apply mouse movement to view
  //Vector3f &rotation = entity.getComponent<Transform>().rotation;
  plr.headAngle.attitude -= rad(mousedy * Environment::getConfig().getSensitivity());
  plr.headAngle.heading  -= rad(mousedx * Environment::getConfig().getSensitivity());
  plr.headAngle.tilt *= 0.8;

  // Restrict rotation in horizontal axis
  plr.headAngle.attitude = Math::clamp(plr.headAngle.attitude, rad(-89.99), rad(89.99));
}

void PlayerSystem::move(Entity &entity, double dtime) {
  (void) dtime;
  Player &plr = entity.getComponent<Player>();
  if (plr.frozen) {
    return;
  }
  bool movingFwd     = Input::isKeyDown(SDL_SCANCODE_W) or Input::isKeyDown(SDL_SCANCODE_UP),
       movingBack    = Input::isKeyDown(SDL_SCANCODE_S) or Input::isKeyDown(SDL_SCANCODE_DOWN),
       strafingLeft  = Input::isKeyDown(SDL_SCANCODE_A) or Input::isKeyDown(SDL_SCANCODE_LEFT),
       strafingRight = Input::isKeyDown(SDL_SCANCODE_D) or Input::isKeyDown(SDL_SCANCODE_RIGHT),
       jumping       = Input::isKeyDown(SDL_SCANCODE_SPACE) or
                       Input::isKeyDown(SDL_SCANCODE_BACKSPACE);
  float rot = plr.headAngle.heading;
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

  movement *= RUNNING_SPEED;
  ctrl.setWalkDirection(movement);

  if (ctrl.onGround()) {
    plr.stepCounter += std::sqrt(movement.x*movement.x + movement.z*movement.z);

    if (plr.stepCounter >= 2.5f) {
      std::uniform_int_distribution<> dis(0, PLAYER_FOOT_SOUND.size()-1);
      entity.getComponent<SoundSource>().playSound(
        Environment::getDataDir() + PLAYER_FOOT_SOUND[dis(generator)]);
      plr.stepCounter -= 2.5f;
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

void PlayerSystem::update(float dtime) {
  mouseLook(*scene->player);
  move(*scene->player, dtime);
}

} /* namespace glPortal */
