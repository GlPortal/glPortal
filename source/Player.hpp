#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <engine/Entity.hpp>
#include <util/Vector2f.hpp>
#include <util/Vector3f.hpp>

namespace glPortal {

const float RUNNING_SPEED = 0.1f;
const float JUMP_SPEED = 0.15f;

class Player: public Entity {
public:
  Player() {
    scale.set(0.5f, 1, 0.5f);

    sensitivity.set(0.25, 0.25);
    velocity.set(0, 0, 0);
    speed = RUNNING_SPEED;

    grounded = false;
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

  Vector3f velocity;
  bool grounded;

  Vector2f sensitivity;
  float speed;
private:
  const int MAX_HEALTH = 100;
  const int MIN_HEALTH = -100;
  int health = MAX_HEALTH;
};

} /* namespace glPortal */

#endif /* PLAYER_HPP */
