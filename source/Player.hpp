#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <engine/Entity.hpp>
#include <engine/core/math/Vector2f.hpp>
#include <engine/core/math/Vector3f.hpp>
#include <array>
#include <random>

namespace glPortal {

const Vector3f PLAYER_SIZE(0.5, 1, 0.5);
const float RUNNING_SPEED = 0.1f;
const float JUMP_SPEED = 0.15f;

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

class Player: public Entity {
public:
  Player() {
    scale.set(PLAYER_SIZE);

    sensitivity.set(0.25, 0.25);
    velocity.set(0, 0, 0);
    speed = RUNNING_SPEED;

    grounded = true;
    std::random_device rd;
	  generator =  std::mt19937(rd());
    playingSound = false;
    stepCounter = 0.0f;
  }

  // Movement
  void mouseLook();
  void move();

  // Health
  int getHealth();
  bool isAlive();
  void heal(int amount);
  void harm(int amount);
  void kill();
  bool getPlayingSound();
  void setPlayingSound(bool state);
  Vector3f velocity;
  bool grounded;

  Vector2f sensitivity;
  float speed;
private:
  const int MAX_HEALTH = 100;
  const int MIN_HEALTH = -100;
  int health = MAX_HEALTH;
  std::mt19937 generator;
  bool playingSound;
  float stepCounter;
};

} /* namespace glPortal */

#endif /* PLAYER_HPP */
