/**
 * \mainpage glPortal
 * \author Simon Jonas Larsen - slars10
 *
 * \par Class diagram:
 * \image html classdiagram.png
 */

#include <GL/glut.h>
#include <cstdio>
#include "Main.hpp"
#include "Box.hpp"
#include "Resources.hpp"

/**
 * \addtogroup Main
 * @{
 * Functions for setting up OpenGL context and window,
 * handling input and updating/drawing entities.
 */

/**
 * Updates all game logic.
 * @param value Value passed from glutTimerFunc. Not used.
 */
void update(int value) {
	if(!paused) {
		float mousedx = (float)(width/2-mousex);  // Mouse distance from center of screen
		float mousedy = (float)(height/2-mousey);

		player.update(CONST_DT, keystates, mousedx, mousedy, map);

		glutWarpPointer(width/2, height/2);
		glutPostRedisplay();
	}
	glutTimerFunc(FRAMETIME, update, 1);
}

/**
 * Redraws the screen. Called by glutDisplayFunc().
 */
void draw() {
	glClear(GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	// Draw scene
	player.setView();
	glEnable(GL_LIGHTING);
	drawPortals();
	map.draw();
	glDisable(GL_LIGHTING);

	player.drawPortalOutlines();
	player.drawShots();

	// Draw crosshair
	Resources::inst().bindTexture(TID_CROSSHAIR);
	glLoadIdentity();

	// Switch to orthographic 2D projection
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0,width,height,0);
	glEnable(GL_BLEND);
	glBegin(GL_QUADS);
		glTexCoord2f(0,0); glVertex3f(width/2-16, height/2-12, 0);
		glTexCoord2f(0,1); glVertex3f(width/2-16, height/2+20, 0);
		glTexCoord2f(1,1); glVertex3f(width/2+16, height/2+20, 0);
		glTexCoord2f(1,0); glVertex3f(width/2+16, height/2-12, 0);
	glEnd();
	glDisable(GL_BLEND);

	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);

	// Swap buffers
	glutSwapBuffers();
}

/**
 * Draws the inside of both portals as well as their oulines and stencils.
 */
void drawPortals() {
	if(player.portalsActive()) {
		Portal *portals = player.getPortals();
		glEnable(GL_STENCIL_TEST);
		for(int i = 0; i < 2; i++) {
			int src = i;		// Source portal index
			int dst = (i+1)%2;  // Destination portal index

			glPushMatrix();
			// Always write to stencil buffer
			glStencilFunc(GL_NEVER, 1, 0xFF);
			glStencilOp(GL_REPLACE, GL_KEEP, GL_KEEP);
			glStencilMask(0xFF);
			glClear(GL_STENCIL_BUFFER_BIT);

			portals[src].drawStencil();

			float dx = player.getX()-portals[src].x;
			float dy = player.getY()-portals[src].y;
			float dz = player.getZ()-portals[src].z;

			glClear(GL_DEPTH_BUFFER_BIT);
			// Only pass stencil test if equal to 1
			glStencilMask(0x00);
			glStencilFunc(GL_EQUAL, 1, 0xFF);

			// Move camera to portal view
			glTranslatef(-dx,-dy,-dz);
			glTranslatef(player.getX(), player.getY(), player.getZ());
			portals[src].rotateFromDir();
			portals[dst].rotateToDir();
			glTranslatef(-portals[dst].x, -portals[dst].y, -portals[dst].z);

			// Draw scene from portal view
			map.drawFromPortal(portals[dst]);
			player.drawPortalOutlines();

			glPopMatrix();
		}
		glDisable(GL_STENCIL_TEST);

		// Draw portal stencils so portals wont be drawn over
		glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
		glClear(GL_DEPTH_BUFFER_BIT);
		player.drawPortalStencils();
		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	}
}

void mouse_moved(int x, int y) {
	mousex = x;
	mousey = y;
}

void mouse_pressed(int button, int state, int x, int y) {
	if(state == GLUT_DOWN) {
		if(button == GLUT_LEFT_BUTTON || button == GLUT_RIGHT_BUTTON) {
			player.mousePressed(button);
		}
	}
}

void key_down(unsigned char key, int x, int y) {
	keystates[key] = true;
	if(key == 27) paused = !paused;
}

void key_up(unsigned char key, int x, int y) {
	keystates[key] = false;
}

void resize(int w, int h) {
	// Setup viewport
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	// Setup projection matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (GLdouble)w/(GLdouble)h, 0.05f, 50.f);

	width = w;
	height = h;

	glMatrixMode(GL_MODELVIEW);
}

/**
 * Sets up the OpenGL context.
 * Loads texture, sets up callbacks and enables lighting, blending etc.
 *
 * @param argc Reference to argument count from main
 * @param argv Arguments from main
 */
void setup(int *argc, char **argv) {
	// Initialize GLUT window
	glutInit(argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_STENCIL);
	glutInitWindowSize(DEFAULT_WIDTH, DEFAULT_HEIGHT);
	glutCreateWindow("glPortal");
	glutSetCursor(GLUT_CURSOR_NONE);
	resize(DEFAULT_WIDTH, DEFAULT_HEIGHT);

	// Reset keystates
	for(int i = 0; i < 256; i++) keystates[i] = false;

	// Setup callback functions
	glutDisplayFunc(draw);
	glutReshapeFunc(resize);
	glutTimerFunc(FRAMETIME, update, 1);
	glutKeyboardFunc(key_down);
	glutKeyboardUpFunc(key_up);
	glutPassiveMotionFunc(mouse_moved);
	glutMouseFunc(mouse_pressed);
	glutWarpPointer(width/2, height/2);

	// Enable textures
	glEnable(GL_TEXTURE_2D);
	// Enable depth testing
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	// Enable back face culling
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	// Enable lighting
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	// Set ambient and diffuse lighting
	GLfloat light_SpecAndAmb[4] = {1.f, 1.f, 1.f, 1.f};
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_SpecAndAmb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_SpecAndAmb);
	// Set blending function for portals
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Load textures from files
	Resources::inst().loadTextures();

	paused = false;
	player.create(2.5, 1, 5);
	map.load("maps/test.map");
}
/**
 * @}
 */

int main(int argc, char **argv) {
	setup(&argc, argv);
	glutMainLoop();

	return EXIT_SUCCESS;
}
