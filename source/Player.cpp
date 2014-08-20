#include "Player.hpp"

namespace glPortal {
  void Player::harm(int amount){
    this->health = this->health - amount;
    if(this->health < minHealth)  this->health = minHealth;
   
  }
  
  void Player::heal(int amount){
    this->health = health + amount;
    if(health > maxHealth) health = maxHealth;
  }

  void Player::kill(){
    health = 0;
  }  

  int Player::getHealth(){
    return health;
  }  
  
  bool Player::isAlive(){
    if(health > 0) return true;
    return false;
  }
} /* namespace glPortal */
