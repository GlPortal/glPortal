#include <math.h>
#include <GL/glut.h>
#include "Player.hpp"

#define RADDEG 57.29577951308232088 // 180/PI
#define PLAYER_MOVESPEED 10

Player::Player() {
	x = y = z = 0.f;
	xrot = yrot = 0.f;
}

void Player::create(float _x, float _y, float _z) {
	x = _x;
	y = _y;
	z = _z;
}

void Player::update(float dt, bool *keystates, float mousedx, float mousedy) {
	// Apply rotation of view
	yrot += mousedx*0.0015f;
	xrot += mousedy*0.0015f;

	// Restrict rotation in x-axis
	if(xrot < -1.5f) xrot = -1.5f;
	if(xrot > 1.5f)  xrot =  1.5f;

	// Move player
	if(keystates['w']) {
		z -= cos(yrot)*PLAYER_MOVESPEED*dt;
		x -= sin(yrot)*PLAYER_MOVESPEED*dt;
	}
	if(keystates['s']) {
		z += cos(yrot)*PLAYER_MOVESPEED*dt;
		x += sin(yrot)*PLAYER_MOVESPEED*dt;
	}
	if(keystates['a']) {
		x -= cos(yrot)*PLAYER_MOVESPEED*dt;
		z += sin(yrot)*PLAYER_MOVESPEED*dt;
	}
	if(keystates['d']) {
		x += cos(yrot)*PLAYER_MOVESPEED*dt;
		z -= sin(yrot)*PLAYER_MOVESPEED*dt;
	}
}

void Player::setView() {
	glRotatef(-xrot*RADDEG, 1,0,0);
	glRotatef(-yrot*RADDEG, 0,1,0);
	glTranslatef(-x, -y, -z);
}
