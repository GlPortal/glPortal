#include <GL/glut.h>
#include <stdio.h>
#include "stb_image.c"
#include "Main.hpp"

void update(int value) {
	glutTimerFunc(FRAMETIME, update, 1);

	float mousedx = (float)(width/2-mousex);  // Mouse distance from center of screen
	float mousedy = (float)(height/2-mousey);

	player.update(CONST_DT, keystates, mousedx, mousedy, map);

	glutWarpPointer(width/2, height/2);
	glutPostRedisplay();
}

void draw() {
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClear(GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	player.setView();

	map.draw(textures);
	drawPortals();

	glutSwapBuffers();
}

void drawPortals() {
	if(player.portalsActive()) {
		glPushMatrix();
		glEnable(GL_STENCIL_TEST);
		glStencilFunc(GL_NEVER, 1, 0xFF);
		glStencilOp(GL_REPLACE, GL_KEEP, GL_KEEP);
		glStencilMask(0xFF);
		glClear(GL_STENCIL_BUFFER_BIT);

		Portal *portals = player.getPortals();
		portals[0].drawStencil();

		glTranslatef(player.getX(), player.getY(), player.getZ()); // Move to orego
		glTranslatef(-portals[0].x, -portals[0].y, -portals[0].z); // Move to first portal
		portals[0].rotateFromDir(); // Rotate
		glTranslatef(portals[0].x-player.getX(),
				     portals[0].y-player.getY(),
					 portals[0].z-player.getZ());
		portals[1].rotateToDir();
		glTranslatef(portals[0].x-portals[1].x, 
				     portals[0].y-portals[1].y,
					 portals[0].z-portals[1].z);
		// Hvis [2] er PD_RIGHT: glTranslatef(-2,0,2);
		// Hvis [2] =  PD_LEFT:  glTranslatef(-2,0,2);
		glTranslatef(-2,0,-2);

		glClear(GL_DEPTH_BUFFER_BIT);
		glStencilMask(0x00);
		glStencilFunc(GL_EQUAL, 1, 0xFF);

		map.draw(textures);

		glDisable(GL_STENCIL_TEST);
		glPopMatrix();
	}
}

void mouse_moved(int x, int y) { mousex = x; mousey = y; }

void key_down(unsigned char key, int x, int y) { keystates[key] = true; }

void key_up(unsigned char key, int x, int y) { keystates[key] = false; }

void resize(int w, int h) {
	// Setup viewport
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	// Setup projection matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (GLdouble)w/(GLdouble)h, 0.1, 50.0);

	width = w;
	height = h;

	glMatrixMode(GL_MODELVIEW);
}

GLuint createTexture(const char *filename) {
	GLuint handle;
	glGenTextures(1, &handle);
	glBindTexture(GL_TEXTURE_2D, handle);

	int w, h, n;
	unsigned char *data = stbi_load(filename, &w, &h, &n, 0);

	if(n == 3)
		gluBuild2DMipmaps(GL_TEXTURE_2D, n, w, h, GL_RGB, GL_UNSIGNED_BYTE, data);
	else if(n == 4)
		gluBuild2DMipmaps(GL_TEXTURE_2D, n, w, h, GL_RGBA, GL_UNSIGNED_BYTE, data);

	stbi_image_free(data);

	// Print any errors
	GLenum error = glGetError();
	while(error != GL_NO_ERROR) {
		printf("Error: %d\n", error);
	}

	return handle;
}

void loadTextures() {
	textures[0] = createTexture("data/wall.png");
	textures[1] = createTexture("data/toxic.png");

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

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
	glutWarpPointer(width/2, height/2);

	// Enable textures
	glEnable(GL_TEXTURE_2D);
	// Enable depth testing
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
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

	// Load textures from files
	loadTextures();

	player.create(2.5, 1, 5);
	map.load("maps/test.map");
}

int main(int argc, char **argv) {
	setup(&argc, argv);
	glutMainLoop();

	return EXIT_SUCCESS;
}
