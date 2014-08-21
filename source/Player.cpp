#include "Player.hpp"

namespace glPortal {

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

} /* namespace glPortal */
