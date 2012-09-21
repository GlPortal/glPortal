#ifndef __PLAYER_HPP
#define __PLAYER_HPP

#include "Map.hpp"
#include "Portal.hpp"
#define RADDEG 57.29577951308232088 // 180/PI

struct Shot {
public:
	Shot() : active(false), x(0), y(0), z(0), xspeed(0), yspeed(0), zspeed(0) { }

	void update(float dt) {
		x += xspeed*dt;
		y += yspeed*dt;
		z += zspeed*dt;
	}

	void draw(GLuint *textures, float xrot, float yrot) {
		glPushMatrix();
		glTranslatef(x,y,z);
		glRotatef(yrot*RADDEG, 0,1,0);
		glRotatef(xrot*RADDEG, 1,0,0);

		glBegin(GL_QUADS);
			glTexCoord2f(0,0); glVertex3f(-0.25, -0.25, 0);
			glTexCoord2f(0,1); glVertex3f(-0.25,  0.25, 0);
			glTexCoord2f(1,1); glVertex3f( 0.25,  0.25, 0);
			glTexCoord2f(1,0); glVertex3f( 0.25, -0.25, 0);
		glEnd();
		glPopMatrix();
	}

	bool active;
	float x,y,z;
	float xspeed, yspeed, zspeed;
};

class Player {
public:
	Player();
	void create(float x, float y, float z);
	void update(float dt, bool *keystates, float mousex, float mousey, Map &map);
	void setView();
	bool portalsActive();
	void drawPortalStencils();
	void drawPortalOutlines(GLuint *textures);

	float getX() { return x; }
	float getY() { return y; }
	float getZ() { return z; }
	Portal *getPortals() { return portals; }

protected:
	float x,y,z; 		// Position
	float xrot, yrot;	// Rotation in x- and y-axis
	float xspeed, yspeed, zspeed;
	bool onGround;
	Portal portals[2];
};

#endif
