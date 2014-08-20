#include "Player.hpp"

namespace glPortal {
  void Player::harm(int amount){
    health = health - amount;
    if(health < MIN_HEALTH) health = MIN_HEALTH;
  }
  
  void Player::heal(int amount){
    health = health + amount;
    if(health > MAX_HEALTH) health = MAX_HEALTH;
  }
  
  bool Player::isAlive(){
    if(health >= 0) return true;
    return false;
  }
} /* namespace glPortal */
