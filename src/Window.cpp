#include "Window.hpp"
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>
#include "Resources.hpp"
#include "Portal.hpp"

void Window::setup(int *argc, char **argv) {
  glutInit(argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_STENCIL);
  glutInitWindowSize(DEFAULT_WIDTH, DEFAULT_HEIGHT);
  glutCreateWindow("glPortal");
  glutSetCursor(GLUT_CURSOR_NONE);
  setSize(DEFAULT_WIDTH, DEFAULT_HEIGHT);
  glewInit();

}

/**
 * Called when the window is resized.
 * Updates the OpenGL viewport and projection matrix.
 *
 * @param w New window width
 * @param h New window height
 */
void Window::setSize(int w, int h) {
  // Setup viewport
  glViewport(0, 0, (GLsizei)w, (GLsizei)h);
  // Setup projection matrix
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(60.0, (GLdouble)w/(GLdouble)h, 0.1f, 50.f);

  this->width = w;
  this->height = h;

  glMatrixMode(GL_MODELVIEW);
}


int Window::getWidth(){
  return this->width;
}

int Window::getHeight(){
  return this->height;
}
