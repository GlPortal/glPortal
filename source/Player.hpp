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
private:
  const int MIN_HEALTH = 100;
  const int MAX_HEALTH = -100;
  int health;
  void harm(int amount);
  void heal(int amount);
  bool isAlive();
};

} /* namespace glPortal */

#endif /* PLAYER_HPP */
