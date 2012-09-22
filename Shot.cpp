#include <math.h>
#include "Shot.hpp"

#define RADDEG 57.29577951308232088 // 180/PI
#define SHOTSPEED 32.0

void Shot::shoot(int _id, float _x, float _y, float _z, float xrot, float yrot) {
	active = true;

	id = _id;
	x = _x;
	y = _y+1.5f;
	z = _z;

	// Approximation of actual vector
	xspeed = -sin(yrot)*cos(xrot)*SHOTSPEED;
	yspeed =  sin(xrot)*SHOTSPEED;
	zspeed = -cos(yrot)*cos(xrot)*SHOTSPEED;
}

void Shot::update(float dt) {
	x += xspeed*dt;
	y += yspeed*dt;
	z += zspeed*dt;
}

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
