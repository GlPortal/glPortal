#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "engine/Entity.hpp"
#include "engine/util/Vector3f.hpp"

namespace glPortal {

class Player: public Entity {
public:
  Player() :
      velocity(0, 0, 0) {
    scale.set(0.5f, 1, 0.5f);
  }
  Vector3f velocity;
  void kill();
  int getHealth();
  void harm(int amount);
  void heal(int amount);
  bool isAlive();
private:
  int maxHealth = 100;
  int minHealth = -100;
  int health = maxHealth;
};

} /* namespace glPortal */

#endif /* PLAYER_HPP */
