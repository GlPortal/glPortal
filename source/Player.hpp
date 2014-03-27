#ifndef __PLAYER_HPP
#define __PLAYER_HPP

#include "GameMap.hpp"
#include "Portal.hpp"
#include "Shot.hpp"
#include "util/Vector3f.hpp"

enum PLAYER_STATE { PS_ALIVE, PS_DYING, PS_WON };

using namespace util;

/**
 * Class for controlling the player and camera view
 */
class Player {
public:
  Player();
  void create(float x, float y, float z);
  void destroy();
  void kill();
  void revive();
  bool isDead();
  void setHasWon();
  bool hasWon();
  PLAYER_STATE getState() { return state; } /**< Gets the player's current state */
  bool isOnGround();
  void setOnGround();
  void setOffGround();
public:
  Vector3f *position;
  Vector3f *rotation;
  Vector3f *velocity;
protected:
  bool onGround;	/**< True if player was standing on ground last frame */
  PLAYER_STATE state; /**< Players current state e.g. alive, dead etc. */
};

#endif
