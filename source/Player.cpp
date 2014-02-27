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

  for(int i = 0; i < 2; i++) {
    portals[i].disable();
    shots[i].active = false;
		
  }
}

/**
 * Updates the player movement vectors based on movement keys,
 * and performs collision with walls and acid pools.
 *
 * @param dt Time in seconds passed since last update
 * @param keystates Array of keyboard key states
 * @param mousedx Movement since last update in horizontal axis
 * @param mousedy Movement since last udpate in vertical axis
 * @param gameMapReference to the Map
 */
void Player::update(float dt, bool *keystates, float mousedx, float mousedy, GameMap &gameMap) {
  // Apply mouse movement to view
  yrot += mousedx*0.0015f;
  xrot += mousedy*0.0015f;

  // Restrict rotation in horizontal axis
  if(xrot < -1.5f) xrot = -1.5f;
  if(xrot >  1.5f) xrot =  1.5f;

  // If player is alive
  if(state == PS_ALIVE) {
    // Reset x and z speed
    xspeed = zspeed = 0.f;
    // Apply gravity to yspeed
    yspeed -= GRAVITY*dt;
    if(yspeed < -MAXSPEED) yspeed = -MAXSPEED;

    // Move forward
    if(keystates['w']) {
      zspeed -= cos(yrot)*PLAYER_MOVESPEED*dt;
      xspeed -= sin(yrot)*PLAYER_MOVESPEED*dt;
    }
    // Move backward
    if(keystates['s']) {
      zspeed += cos(yrot)*PLAYER_MOVESPEED*dt;
      xspeed += sin(yrot)*PLAYER_MOVESPEED*dt;
    }
    // Strafe left
    if(keystates['a']) {
      xspeed -= cos(yrot)*PLAYER_MOVESPEED*dt;
      zspeed += sin(yrot)*PLAYER_MOVESPEED*dt;
    }
    // Strafe right
    if(keystates['d']) {
      xspeed += cos(yrot)*PLAYER_MOVESPEED*dt;
      zspeed -= sin(yrot)*PLAYER_MOVESPEED*dt;
    }
    // Jump if space is pressed and player is standing on ground
    if(keystates[' '] && onGround) {
      yspeed = JUMPPOWER;
    }
    // Disable portals if R is held
    if(keystates['r']) {
      portals[0].disable();
      portals[1].disable();
    }

    float newx = x + xspeed*dt;
    float newy = y + yspeed*dt;
    float newz = z + zspeed*dt;


    // Check for collision in x-axis
    Box bbox;
    bbox.set(newx-0.5, y, z-0.5, newx+0.5, y+1.8, z+0.5);
    if(gameMap.collidesWithWall(bbox) == false || portals[0].inPortal(bbox) || portals[1].inPortal(bbox)) {
      x = newx;
    }
    // Check for collision in y-axis
    bbox.set(x-0.5, y, newz-0.5, x+0.5, y+1.8, newz+0.5);
    if(gameMap.collidesWithWall(bbox) == false || portals[0].inPortal(bbox) || portals[1].inPortal(bbox)) {
      z = newz;
    }
    // Check for collision in z-axis
    bbox.set(x-0.5, newy, z-0.5, x+0.5, newy+1.8, z+0.5);
    onGround = false;
    if(gameMap.collidesWithWall(bbox) == false || portals[0].inPortal(bbox) || portals[1].inPortal(bbox)) {
      y = newy;
    } else {
      // If player was falling it means must have hit the ground
      if(yspeed < 0) {
	onGround = true;
      }
      yspeed = 0.f;
    }

    // Check if player has fallen into an acid pool
    bbox.set(x-0.5, y, z-0.5, x+0.5, y+1.8, z+0.5);
    if(gameMap.collidesWithAcid(bbox) == true) {
      state = PS_DYING;
    }

    // Check if player has taken the cake
    if(gameMap.collidesWithCake(bbox) == true) {
      state = PS_WON;
    }

    // Check if player has entered a portal
    if(portalsActive()) {
      for(int i = 0; i < 2; i++) {
	if(portals[i].throughPortal(x,y+0.9f,z)) {
	  // Calculate rotation between portals
	  float rotation = 0.f;
	  rotation += portals[i].getToRotation()*DEGRAD;
	  rotation += portals[(i+1)%2].getFromRotation()*DEGRAD;
	  yrot += rotation;
	  // Distance from portal to player
	  float xdist = x - portals[i].x;
	  float zdist = z - portals[i].z;
	  // Calculate this distance when rotated
	  float nxdist = xdist*cos(rotation) + zdist*sin(rotation);
	  float nzdist = zdist*cos(rotation) - xdist*sin(rotation);
	  // Move player to destination portal
	  x = portals[(i+1)%2].x + nxdist;
	  z = portals[(i+1)%2].z + nzdist;
	  y += portals[(i+1)%2].y - portals[i].y;
	}
      }
    }
  }
  // If player is dying
  else if(state == PS_DYING) {
    y -= 0.60f*dt;
		
  }

  // Update shots and check their collision with walls
  for(int i = 0; i < 2; i++) {
    if(shots[i].active) {
      shots[i].update(dt);

      Box sbox;
      if(gameMap.pointInWall(shots[i].x, shots[i].y, shots[i].z, &sbox)) {
	shots[i].update(-dt); // Reverse time to before collision
	// Collision really should be interpolated instead
	if(sbox.type == TID_WALL) {
	  portals[i].placeOnBox(sbox, shots[i].x, shots[i].y, shots[i].z, gameMap);
	}
	shots[i].active = false;
      }
    }
  }
}

/**
 * Called when a mouse button is pressed down
 * @param button Mouse button being pressed
 */
void Player::mousePressed(int button) {
  if(state != PS_ALIVE) return;

  switch(button) {
  case GLUT_LEFT_BUTTON:
    // Shoot blue portal
    shots[0].shoot(0,x,y,z,xrot,yrot);
    break;
  case GLUT_RIGHT_BUTTON:
    // Shoot orange portal
    shots[1].shoot(1,x,y,z,xrot,yrot);
    break;
  case GLUT_MIDDLE_BUTTON:
    // Disable both portals
    portals[0].disable();
    portals[1].disable();
    break;
  }
}

/**
 * Updates the view according to the player's
 *
 * Updates the current MODELVIEW matrix with the inverse
 * translation and rotation of the player's
 */
void Player::setView() {
  glRotatef(-xrot*RADDEG, 1,0,0);
  glRotatef(-yrot*RADDEG, 0,1,0);
  glTranslatef(-x, -(y+1.7f), -z);
}

/**
 * Returns true if both portals are active
 */
bool Player::portalsActive() {
  return (portals[0].active && portals[1].active);
}

/**
 * Draws stencil meshes for both portals
 */
void Player::drawPortalStencils() {
  portals[0].drawStencil();
  portals[1].drawStencil();
}

/**
 * Draws colored outlines for both portals
 */
void Player::drawPortalOutlines() {
  if(portals[0].active) portals[0].drawOutline(PC_BLUE);
  if(portals[1].active) portals[1].drawOutline(PC_ORANGE);
}

/**
 * Draws both portal shots
 */
void Player::drawShots() {
  if(shots[0].active) shots[0].draw(xrot, yrot);
  if(shots[1].active) shots[1].draw(xrot, yrot);
}

