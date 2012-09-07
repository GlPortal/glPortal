#include <GL/glut.h>
#include <stdio.h>
#include "Main.hpp"

void update(int value) {
	glutTimerFunc(FRAMETIME, update, 1);

	float mousedx = (float)(width/2-mousex);  // Mouse distance from center of screen
	float mousedy = (float)(height/2-mousey);

	player.update(CONST_DT, keystates, mousedx, mousedy);

	glutWarpPointer(width/2, height/2);
	glutPostRedisplay();
}

void draw() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	player.setView();
	map.draw();

	glutSwapBuffers();
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

void setup(int *argc, char **argv) {
	glutInit(argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(DEFAULT_WIDTH, DEFAULT_HEIGHT);
	glutCreateWindow("fps test");
	glutSetCursor(GLUT_CURSOR_NONE);

	for(int i = 0; i < 256; i++) keystates[i] = false;

	glutPassiveMotionFunc(mouse_moved);
	glutDisplayFunc(draw);
	glutReshapeFunc(resize);
	glutTimerFunc(FRAMETIME, update, 1);
	glutKeyboardFunc(key_down);
	glutKeyboardUpFunc(key_up);

	glClearColor(0.0, 0.0, 0.0, 0.0);
	resize(DEFAULT_WIDTH, DEFAULT_HEIGHT);
	glutWarpPointer(width/2, height/2);

	player.create(0, 1.8, 0);
}

int main(int argc, char **argv) {
	setup(&argc, argv);
	glutMainLoop();

	return EXIT_SUCCESS;
}
