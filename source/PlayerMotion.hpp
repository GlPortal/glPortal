#ifndef PLAYER_MOTION_HPP
#define PLAYER_MOTION_HPP

#include <engine/Entity.hpp>
#include <engine/component/Transform.hpp>
#include <engine/core/math/Vector2f.hpp>
#include <engine/core/math/Vector3f.hpp>
#include <array>
#include <random>

namespace glPortal {

const Vector3f PLAYER_SIZE(0.5, 1, 0.5);
const float RUNNING_SPEED = 0.1f;
const float JUMP_SPEED = 0.15f;
const float HURT_VELOCITY = 0.18f;

const std::array<const std::string,2> PLAYER_PANTING_SOUND =
{
	"/audio/sfx/character/fem_panting_1.ogg",
	"/audio/sfx/character/fem_panting_2.ogg"
};

const std::array<const std::string,2> PLAYER_JUMP_SOUND =
{
	"/audio/sfx/character/fem_jump_1.ogg",
	"/audio/sfx/character/fem_jump_2.ogg"
};

const std::array<const std::string,2> PLAYER_FALL_SOUND =
{
	"/audio/sfx/character/fem_fall_1.ogg",
	"/audio/sfx/character/fem_fall_2.ogg"
};

const std::array<const std::string,6> PLAYER_FOOT_SOUND =
{
  "/audio/sfx/character/fem_foot_1.ogg",
  "/audio/sfx/character/fem_foot_2.ogg",
  "/audio/sfx/character/fem_foot_3.ogg",
  "/audio/sfx/character/fem_foot_4.ogg",
  "/audio/sfx/character/fem_foot_5.ogg",
  "/audio/sfx/character/fem_foot_6.ogg"
};

class PlayerMotion : public Component {
public:
  PlayerMotion(Entity &ent) :
    Component(ent),
    flying(false),
    frozen(false),
    noclip(false) {
    entity.getComponent<Transform>().scale = PLAYER_SIZE;

    velocity.set(0, 0, 0);
    speed = RUNNING_SPEED;

    grounded = true;
    std::random_device rd;
    generator =  std::mt19937(rd());
    stepCounter = 0.0f;
  }

  // Movement
  void mouseLook();
  void move(float dtime);

  Vector3f velocity;
  bool grounded;
  bool flying, noclip, frozen;

  float speed;
private:
  std::mt19937 generator;
  float stepCounter;
};

} /* namespace glPortal */

#endif /* PLAYER_MOTION_HPP */
