#include "Shot.hpp"

#include <cmath>
#include "Resources.hpp"

#define RADDEG 57.29577951308232088 // 180/PI
#define SHOTSPEED 32.0

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
void Shot::shoot(int id, float x, float y, float z, float xrot, float yrot) {
	active = true;

	this->id = id;
	this->x = x;
	this->y = y+1.5f;
	this->z = z;

	// Approximation of actual vector
	xspeed = -sin(yrot)*cos(xrot)*SHOTSPEED;
	yspeed =  sin(xrot)*SHOTSPEED;
	zspeed = -cos(yrot)*cos(xrot)*SHOTSPEED;
}

/**
 * Updates the shot position based on its velocity vector.
 * 
 * @param dt Time since last update in seconds
 */
void Shot::update(float dt) {
	x += xspeed*dt;
	y += yspeed*dt;
	z += zspeed*dt;
}

/**
 * Draws the shot as a billboard sprite
 *
 * @param xrot Camera's current rotation in X-axis
 * @param yrot Camera's current rotation in Y-axis
 */
void Shot::draw(float xrot, float yrot) {
	glPushMatrix();
	glTranslatef(x,y,z);
	// Reverse camera rotation to make sprite always
	// face the camera
	glRotatef(yrot*RADDEG, 0,1,0);
	glRotatef(xrot*RADDEG, 1,0,0);

	Resources::inst().bindTexture(TID_BALLS);
	glEnable(GL_BLEND);
	float xoffset = id*0.5f;
	glBegin(GL_QUADS);
		glTexCoord2f(xoffset+0.5f, 0); glVertex3f( 0.25, -0.25, 0);
		glTexCoord2f(xoffset+0.5f, 1); glVertex3f( 0.25,  0.25, 0);
		glTexCoord2f(xoffset,      1); glVertex3f(-0.25,  0.25, 0);
		glTexCoord2f(xoffset,      0); glVertex3f(-0.25, -0.25, 0);
	glEnd();
	glDisable(GL_BLEND);
	glPopMatrix();
}
