#include <math.h>
#include "Shot.hpp"

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
 * @param Time since last update in seconds
 */
void Shot::update(float dt) {
	x += xspeed*dt;
	y += yspeed*dt;
	z += zspeed*dt;
}

/**
 * Draws the shot as a billboard sprite
 *
 * @param Array of texture handles
 * @param Camera's current rotation in X-axis
 * @param Camera's current rotation in Y-axis
 */
void Shot::draw(GLuint *textures, float xrot, float yrot) {
	glPushMatrix();
	glTranslatef(x,y,z);
	// Reverse camera rotation to make sprite always
	// face the camera
	glRotatef(yrot*RADDEG, 0,1,0);
	glRotatef(xrot*RADDEG, 1,0,0);

	glBindTexture(GL_TEXTURE_2D, textures[6]);
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

/**
 * Places a given portal on a given box from this shot.
 *
 * @param box The box to place a portal on
 * @param portal The portal to place
 */
void Shot::placePortal(Box &box, Portal &portal) {
	float dist[4];
	int min;

	dist[0] = x - box.x1; // Distance from left face to x
	dist[1] = box.x2 - x; // Distance from right face to x
	dist[2] = z - box.z1; // Distance from back face to z
	dist[3] = box.z2 - z; // Distance from front face to z

	// Find smallest distance
	min = 0;	
	for(int i = 1; i < 4; i++) {
		if(dist[i] < dist[min]) {
			min = i;
		}
	}

	// Left face
	if(min == 0) {
		portal.place(box.x1,y,z, PD_LEFT);
	// Right face
	} else if(min == 1) {
		portal.place(box.x2,y,z, PD_RIGHT);
	// Back face
	} else if(min == 2) {
		portal.place(x,y,box.z1, PD_BACK);
	// Front Face
	} else {
		portal.place(x,y,box.z2, PD_FRONT);
	}
}
