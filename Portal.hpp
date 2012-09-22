#ifndef __PORTAL_HPP
#define __PORTAL_HPP

#include <cmath>

enum PORTAL_DIR { PD_RIGHT, PD_FRONT, PD_LEFT, PD_BACK, PD_NONE }; /**< Possible direction a portal can face */
enum PORTAL_COLOR { PC_BLUE, PC_ORANGE }; /**< Colors a portal can have */

/**
 * Class for controlling and drawing portals
 */
class Portal {
public:
	float x,y,z;
	PORTAL_DIR dir; /**< Direction portal is facing */
	bool active;	/**< True if the portal is currently active */

	Portal() : x(0), y(0), z(0), dir(PD_NONE), active(false) { }

	/**
	 * Places and activates the portal.
	 * Coordinates will be rounded for better alignment with walls and floor.
	 *
	 * @param x New x-coordinate
	 * @param y New y-coordinate
	 * @param z New z-coordinate
	 * @param dir New direction
	 */
	void place(float x, float y, float z, PORTAL_DIR dir) {
		this->x = round(x);
		this->y = round(y)+0.25f;
		this->z = round(z);

		this->dir = dir;
		active = true;
	}
	
	/**
	 * Disables the portal
	 */
	void disable() {
		active = false;
	}

	/**
	 * Rotates current matrix to make matrix point in positive Z-direction.
	 */
	void rotateToDir() {
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
	void rotateFromDir() {
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
	void drawStencil() {
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
	void drawOutline(PORTAL_COLOR color, GLuint *textures) {
		glPushMatrix();

		glTranslatef(x,y,z);
		rotateFromDir();

		// Bind blue of orange portal texture
		if(color == PC_BLUE) glBindTexture(GL_TEXTURE_2D, textures[3]);
		else glBindTexture(GL_TEXTURE_2D, textures[4]);

		glBegin(GL_QUADS);
		glNormal3f(0,0,1);
		glTexCoord2f(0.207, 0); glVertex3f(-0.84, 1.35, 0.002);
		glTexCoord2f(0.207, 1); glVertex3f(-0.84,-1.35, 0.002);
		glTexCoord2f(0.793, 1); glVertex3f( 0.84,-1.35, 0.002);
		glTexCoord2f(0.793, 0); glVertex3f( 0.84, 1.35, 0.002);
		glEnd();

		glPopMatrix();
	}
};

#endif
