#include "Player.hpp"
#include <cmath>
#include <cstdio>
#include <GL/freeglut.h>

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
  this->x = x;
  this->y = y;
  this->z = z;

  xrot = yrot = 0.f;
  xspeed = yspeed = zspeed = 0.f;
  onGround = false;
  state = PS_ALIVE;
}

void Player::setX(float x){
  this->x = x;

}

void Player::setY(float y){
  this->y = y;
}

void Player::setZ(float z){
  this->z = z;
}

void Player::setPosition(float x, float y, float z) {
  this->x = x;
  this->y = y;
  this->z = z;
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
