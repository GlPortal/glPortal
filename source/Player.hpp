#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "engine/Entity.hpp"
#include "engine/util/Vector3f.hpp"

namespace glPortal {

class Player: public Entity {
public:
  Player() :
      velocity(0, 0, 0),
      grounded(false) {
    scale.set(0.5f, 1, 0.5f);
  }
  int getHealth();
  bool isAlive();
  void heal(int amount);
  void harm(int amount);
  void kill();

  Vector3f velocity;
  bool grounded;
private:
  const int MAX_HEALTH = 100;
  const int MIN_HEALTH = -100;
  int health = MAX_HEALTH;
};

} /* namespace glPortal */

#endif /* PLAYER_HPP */
