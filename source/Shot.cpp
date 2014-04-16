#include "Shot.hpp"

#include <cmath>
#include "engine/Resources.hpp"

#define RADDEG 57.29577951308232088 // 180/PI
#define SHOTSPEED 16.0

/**
 * Shoots the portal gun from a given point in a given direction.
 *
 * @param id ID of the portal. Either 0 or 1
 * @param x X-coordinate to shoot from
 * @param y Y-coordinate to shoot from
 * @param z Z-coordinate to shoot from
 * @param xrot Gun's rotation in X-axis
 * @param yrot Gun's rotation in Y-axis
 */
void Shot::shoot(int id, float x, float y, float z, float pitch, float yaw) {
	active = true;

	this->id = id;
	this->position.x = x;
	this->position.y = y+1.5f;
	this->position.z = z;

	// Approximation of actual vector
	this->velocity.x = -sin(yaw) * cos(pitch) * SHOTSPEED;
	this->velocity.y =  sin(pitch) * SHOTSPEED;
	this->velocity.z = -cos(yaw) * cos(pitch) * SHOTSPEED;
}

/**
 * Updates the shot position based on its velocity vector.
 * 
 * @param dt Time since last update in seconds
 */
void Shot::update(float dt) {
	position.x += velocity.x * dt;
	position.y += velocity.y * dt;
	position.z += velocity.z * dt;
}

/**
 * Draws the shot as a billboard sprite
 *
 * @param xrot Camera's current rotation in X-axis
 * @param yrot Camera's current rotation in Y-axis
 */
void Shot::draw(float pitch, float yaw) {
	glPushMatrix();
	glTranslatef(position.x, position.y, position.z);
	// Reverse camera rotation to make sprite always
	// face the camera
	glRotatef(yaw * RADDEG, 0,1,0);
	glRotatef(pitch * RADDEG, 1,0,0);

  glEnable(GL_TEXTURE_2D);
	  Resources::inst().bindTexture(TID_BALLS);
	  float xoffset = id*0.5f;
	  glBegin(GL_QUADS);
		  glTexCoord2f(xoffset+0.5f, 0); glVertex3f( 0.25, -0.25, 0);
		  glTexCoord2f(xoffset+0.5f, 1); glVertex3f( 0.25,  0.25, 0);
		  glTexCoord2f(xoffset,      1); glVertex3f(-0.25,  0.25, 0);
		  glTexCoord2f(xoffset,      0); glVertex3f(-0.25, -0.25, 0);
	  glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}

