#include <math.h>
#include <stdio.h>
#include <GL/glut.h>
#include "Player.hpp"

#define RADDEG 57.29577951308232088 // 180/PI
#define PLAYER_MOVESPEED 6.0
#define GRAVITY 16.0
#define MAXSPEED 10.0
#define JUMPPOWER 7.0

Player::Player() {
	x = y = z = 0.f;
	xrot = yrot = 0.f;
	onGround = false;
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
	onGround = false;
}

/**
 * Updates the player movement vectors based on movement keys,
 * and performs collision with walls and acid pools.
 *
 * @param dt Time in seconds passed since last update
 * @param keystates Array of keyboard key states
 * @param mousedx Movement since last update in horizontal axis
 * @param mousedy Movement since last udpate in vertical axis
 * @param Reference to the Map
 */
void Player::update(float dt, bool *keystates, float mousedx, float mousedy, Map &map) {
	// Apply rotation of view
	yrot += mousedx*0.0015f;
	xrot += mousedy*0.0015f;

	// Restrict rotation in x-axis
	if(xrot < -1.5f) xrot = -1.5f;
	if(xrot >  1.5f) xrot =  1.5f;

	// Reset x and z speed
	xspeed = zspeed = 0.f;
	// Apply gravity to yspeed
	yspeed -= GRAVITY*dt;
	if(yspeed < -MAXSPEED) yspeed = -MAXSPEED;

	// Move forward
	if(keystates['w']) {
		zspeed -= cos(yrot)*PLAYER_MOVESPEED;
		xspeed -= sin(yrot)*PLAYER_MOVESPEED;
	}
	// Move backward
	if(keystates['s']) {
		zspeed += cos(yrot)*PLAYER_MOVESPEED;
		xspeed += sin(yrot)*PLAYER_MOVESPEED;
	}
	// Strafe left
	if(keystates['a']) {
		xspeed -= cos(yrot)*PLAYER_MOVESPEED;
		zspeed += sin(yrot)*PLAYER_MOVESPEED;
	}
	// Strafe right
	if(keystates['d']) {
		xspeed += cos(yrot)*PLAYER_MOVESPEED;
		zspeed -= sin(yrot)*PLAYER_MOVESPEED;
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
	Box xbox(newx-0.5, y, z-0.5, newx+0.5, y+1.8, z+0.5);
	if(map.collidesWithWall(xbox) == false) {
		x = newx;
	}
	// Check for collision in y-axis
	Box zbox(x-0.5, y, newz-0.5, x+0.5, y+1.8, newz+0.5);
	if(map.collidesWithWall(zbox) == false) {
		z = newz;
	}
	// Check for collision in z-axis
	Box ybox(x-0.5, newy, z-0.5, x+0.5, newy+1.8, z+0.5);
	onGround = false;
	if(map.collidesWithWall(ybox) == false) {
		y = newy;
	} else {
		if(yspeed < 0) {
			onGround = true;
		}
		yspeed = 0.f;
	}

	// Update shots
	for(int i = 0; i < 2; i++) {
		if(shots[i].active) {
			shots[i].update(dt);

			Box sbox;
			if(map.pointInWall(shots[i].x, shots[i].y, shots[i].z, &sbox)) {
				if(sbox.type == BT_WALL) {
					shots[i].placePortal(sbox, portals[i]);
				}
				shots[i].active = false;
			}
		}
	}
}

/**
 * Called when a mouse button is pressed down
 * @param Mouse button being pressed
 */
void Player::mousePressed(int button) {
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

/*
 * Draws colored outlines for both portals
 *
 * @param textures Pointer to the array of texture handles
 */
void Player::drawPortalOutlines(GLuint *textures) {
	glEnable(GL_BLEND);
	if(portals[0].active) portals[0].drawOutline(PC_BLUE,   textures);
	if(portals[1].active) portals[1].drawOutline(PC_ORANGE, textures);
	glDisable(GL_BLEND);
}

/**
 * Draws both portal shots
 *
 * @param textures Pointer to the array of texture handles
 */
void Player::drawShots(GLuint *textures) {
	if(shots[0].active) shots[0].draw(textures, xrot, yrot);
	if(shots[1].active) shots[1].draw(textures, xrot, yrot);
}
