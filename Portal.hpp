#ifndef __PORTAL_HPP
#define __PORTAL_HPP

enum PORTAL_DIR { PD_RIGHT, PD_FRONT, PD_LEFT, PD_BACK, PD_NONE };

class Portal {
public:
	Portal() : x(0), y(0), z(0), dir(PD_NONE), active(false) { }

	void set(float _x, float _y, float _z, PORTAL_DIR _dir) {
		x = _x; y = _y; z = _z; dir = _dir; active = true;
	}

	void disable() { active = false; }

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

	void drawOutline(GLuint *textures) {

	}

	float x,y,z;
	PORTAL_DIR dir;
	bool active;
};

#endif
