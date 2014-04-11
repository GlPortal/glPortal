#include "Player.hpp"
#include "engine/Environment.hpp"
#include <cmath>
#include <cstdio>

using namespace glPortal::engine;

#define RADDEG 57.29577951308232088 // 180/PI
#define DEGRAD 0.017453292519943296 // PI/180
#define PLAYER_MOVESPEED 330
#define GRAVITY 16.0
#define MAXSPEED 10.0
#define JUMPPOWER 7.0

const int   Player::MOVE_SPEED = 330;
const float Player::MAX_SPEED  = 50.0;
const float Player::JUMP_POWER = 7.0;

Player::Player() {
  create(0.f, 0.f, 0.f);
}

void Player::create(float x, float y, float z) {
  position.set(x, y, z);
  onGround = false;
  state = PS_ALIVE;
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

bool Player::isOnGround(){ 
  return this->onGround;
}

void Player::setOnGround(){
  this->onGround = true;
}

void Player::setOffGround(){
  this->onGround = false;
}

void Player::moveForward(){
  velocity.z -= cos(rotation.y)*Player::MOVE_SPEED*Environment::FRAME_TIME/1000;
  velocity.x -= sin(rotation.y)*Player::MOVE_SPEED*Environment::FRAME_TIME/1000;
}

void Player::moveBackward(){
  velocity.z += cos(rotation.y)*Player::MOVE_SPEED*Environment::FRAME_TIME/1000;
  velocity.x += sin(rotation.y)*Player::MOVE_SPEED*Environment::FRAME_TIME/1000;
}

void Player::moveLeft(){
  velocity.x -= cos(rotation.y)*Player::MOVE_SPEED*Environment::FRAME_TIME/1000;
  velocity.z += sin(rotation.y)*Player::MOVE_SPEED*Environment::FRAME_TIME/1000;
}

void Player::moveRight(){
  velocity.x += cos(rotation.y)*Player::MOVE_SPEED*Environment::FRAME_TIME/1000;
  velocity.z -= sin(rotation.y)*Player::MOVE_SPEED*Environment::FRAME_TIME/1000;
}

