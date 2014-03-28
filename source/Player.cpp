#include "Player.hpp"
#include <cmath>
#include <cstdio>

#define RADDEG 57.29577951308232088 // 180/PI
#define DEGRAD 0.017453292519943296 // PI/180
#define PLAYER_MOVESPEED 330
#define GRAVITY 16.0
#define MAXSPEED 10.0
#define JUMPPOWER 7.0

Player::Player() {
  create(0.f, 0.f, 0.f);
}

void Player::kill(){
  state = PS_DYING;
}

void Player::revive(){
  state = PS_ALIVE;
}

bool Player::isDead(){
  if(state == PS_ALIVE || state == PS_WON){
    return false;
  }
  return true;
}

void Player::setHasWon(){
  state = PS_WON;
}

bool Player::hasWon(){
  if(state == PS_WON){
    return true;
  }
}

/**
 * Sets the position of the player
 * @param x New X-coordinate
 * @param y New Y-coordinate
 * @param z New Z-coordinate
 */
void Player::create(float x, float y, float z) {
  position = new Vector3f(x, y, z);
  rotation = new Vector3f(0, 0, 0);
  velocity = new Vector3f(0, 0, 0);
  onGround = false;
  state = PS_ALIVE;
}

void Player::destroy() {
  delete position;
  delete rotation;
  delete velocity;
}

bool Player::isOnGround(){ 
  return this->onGround;
}

void Player::setOnGround(){
  this->onGround = true;
}

void Player::setOffGround(){
  this->onGround = false;
}
