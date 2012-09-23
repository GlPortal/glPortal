#include <cmath>
#include <GL/glut.h>
#include "Portal.hpp"
#include "Resources.hpp"

/**
 * Places and activates the portal.
 * Coordinates will be rounded for better alignment with walls and floor.
 *
 * @param x New x-coordinate
 * @param y New y-coordinate
 * @param z New z-coordinate
 * @param dir New direction
 */
void Portal::place(float x, float y, float z, PORTAL_DIR dir) {
	this->x = round(x);
	this->y = round(y)+0.25f;
	this->z = round(z);

	this->dir = dir;
	active = true;
}

/**
 * Places the portal on a box based on the shot's position
 * when the collision occured.
 *
 * \param box The box to place portal on
 * \param hitx X-coordinate of shot-box collision
 * \param hity Y-coordinate of shot-box collision
 * \param hitz Z-coordinate of shot-box collision
 */
void Portal::placeOnBox(Box &box, float hitx, float hity, float hitz) {
	float dist[4];
	int min;

	// Calculate distance from shot to planes
	dist[0] = hitx - box.x1; // Distance from left face to x
	dist[1] = box.x2 - hitx; // Distance from right face to x
	dist[2] = hitz - box.z1; // Distance from back face to z
	dist[3] = box.z2 - hitz; // Distance from front face to z

	// Find smallest distance
	min = 0;	
	for(int i = 1; i < 4; i++) {
		if(dist[i] < dist[min]) {
			min = i;
		}
	}

	// Portal on the YZ-plane
	if(min <= 1) {
		// Make sure box is wide enough
		if(box.z2 - box.z1 < 2) return;

		// Left face
		if(min == 0) {
			place(box.x1, hity, hitz, PD_LEFT);
		// Right face
		} else {
			place(box.x2, hity, hitz, PD_RIGHT);
		}

	// Portal on the XY-plane
	} else {
		// Make sure box is wide enough
		if(box.x2 - box.x1 < 2.f) return;

		// Back face
		if(min == 2) {
			place(hitx, hity, box.z1, PD_BACK);
		// Front Face
		} else {
			place(hitx, hity, box.z2, PD_FRONT);
		}
	}
}

/**
 * Disables the portal
 */
void Portal::disable() {
	active = false;
}

/**
 * Rotates current matrix to make matrix point in positive Z-direction.
 */
void Portal::rotateToDir() {
	switch(dir) {
		case PD_RIGHT:
			glRotatef(90.f, 0,1,0);
			break;
		case PD_FRONT:
			glRotatef(180.f, 0,1,0);
			break;
		case PD_LEFT:
			glRotatef(-90.f, 0,1,0);
			break;
	}
}

/**
 * Rotates current matrix to make Z-axis point in portal's direction.
 */
void Portal::rotateFromDir() {
	switch(dir) {
		case PD_LEFT:
			glRotatef(-90.f, 0,1,0);
			break;
		case PD_BACK:
			glRotatef(180.f, 0,1,0);
			break;
		case PD_RIGHT:
			glRotatef(90.f, 0,1,0);
			break;
	}
}

/**
 * Draws a round stencil in the portal.
 * Used for stencil buffer effects.
 */
void Portal::drawStencil() {
	glPushMatrix();

	glTranslatef(x,y,z);
	rotateFromDir();

	glBegin(GL_TRIANGLE_FAN);
	glVertex3f( 0.00, 0.00, 0.001);

	glVertex3f( 0.00, 1.25, 0.001);
	glVertex3f(-0.29, 1.15, 0.001);
	glVertex3f(-0.53, 0.88, 0.001);
	glVertex3f(-0.69, 0.48, 0.001);

	glVertex3f(-0.75, 0.00, 0.001);
	glVertex3f(-0.69,-0.48, 0.001);
	glVertex3f(-0.53,-0.88, 0.001);
	glVertex3f(-0.29,-1.15, 0.001);

	glVertex3f( 0.00,-1.25, 0.001);
	glVertex3f( 0.29,-1.15, 0.001);
	glVertex3f( 0.53,-0.88, 0.001);
	glVertex3f( 0.69,-0.48, 0.001);

	glVertex3f( 0.75, 0.00, 0.001);
	glVertex3f( 0.69, 0.48, 0.001);
	glVertex3f( 0.53, 0.88, 0.001);
	glVertex3f( 0.29, 1.15, 0.001);

	glVertex3f( 0.00, 1.25, 0.001);
	glEnd();

	glPopMatrix();
}

/**
 * Draws the colored outline of a portal.
 *
 * @param color Color of the portal. Either PC_BLUE or PC_ORANGE
 * @param textures Array of texture handles
 */
void Portal::drawOutline(PORTAL_COLOR color) {
	glPushMatrix();

	glTranslatef(x,y,z);
	rotateFromDir();

	// Bind blue of orange portal texture
	if(color == PC_BLUE)
		Resources::inst().bindTexture(TID_BLUEPORTAL);
	else
		Resources::inst().bindTexture(TID_ORANGEPORTAL);

	glBegin(GL_QUADS);
	glNormal3f(0,0,1);
	glTexCoord2f(0.207, 0); glVertex3f(-0.84, 1.35, 0.002);
	glTexCoord2f(0.207, 1); glVertex3f(-0.84,-1.35, 0.002);
	glTexCoord2f(0.793, 1); glVertex3f( 0.84,-1.35, 0.002);
	glTexCoord2f(0.793, 0); glVertex3f( 0.84, 1.35, 0.002);
	glEnd();

	glPopMatrix();
}
