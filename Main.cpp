#include <GL/glut.h>
#include <stdio.h>
#include "stb_image.c"
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
	map.draw(textures);

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
	glutInit(argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(DEFAULT_WIDTH, DEFAULT_HEIGHT);
	glutCreateWindow("glPortal");
	glutSetCursor(GLUT_CURSOR_NONE);

	for(int i = 0; i < 256; i++) keystates[i] = false;

	glutDisplayFunc(draw);
	glutReshapeFunc(resize);
	glutTimerFunc(FRAMETIME, update, 1);
	glutKeyboardFunc(key_down);
	glutKeyboardUpFunc(key_up);
	glutPassiveMotionFunc(mouse_moved);
	glutWarpPointer(width/2, height/2);

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK); // Cull back faces

	glClearColor(0.0, 0.0, 0.0, 0.0);
	resize(DEFAULT_WIDTH, DEFAULT_HEIGHT);

	loadTextures();

	player.create(0, 1.8, 0);
}

int main(int argc, char **argv) {
	setup(&argc, argv);
	glutMainLoop();

	return EXIT_SUCCESS;
}
