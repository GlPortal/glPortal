#ifndef __PLAYER_HPP
#define __PLAYER_HPP

#include "GameMap.hpp"
#include "Portal.hpp"
#include "Shot.hpp"

enum PLAYER_STATE { PS_ALIVE, PS_DYING, PS_WON };

/**
 * Class for controlling the player and camera view
 */
class Player {
public:
  Player();
  void create(float x, float y, float z);
  void kill();
  void revive();
  bool isDead();
  void setHasWon();
  bool hasWon();
  void setPosition(float x, float y, float z);
  void setX(float x);
  void setY(float y);
  void setZ(float z);
  float getX() { return x; } /**< Get the X-coordinate of the Player */
  float getY() { return y; } /**< Get the Y-coordinate of the Player */
  float getZ() { return z; } /**< Get the Z-coordinate of the Player */
  PLAYER_STATE getState() { return state; } /**< Gets the player's current state */
  bool isOnGround();
  void setOnGround();
  void setOffGround();
protected:
  float x,y,z;  // Position
  float xrot; /**< View rotation in X-axis */
  float yrot;	/**< View rotation in Y-axis */
  float xspeed, yspeed, zspeed; // Velocity in x-, y- and z-axis
  bool onGround;	/**< True if player was standing on ground last frame */
  PLAYER_STATE state; /**< Players current state e.g. alive, dead etc. */
};

#endif
