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
  glutWarpPointer(width/2, height/2); // Center pointer  

  if (glutGameModeGet(GLUT_GAME_MODE_POSSIBLE)){
    glutGameModeString("1680x1050:32@60");
    glutEnterGameMode();
  }
  else {
    printf("Game mode not available using default.\n");
  } 
  glutSetCursor(GLUT_CURSOR_NONE);
  setSize(DEFAULT_WIDTH, DEFAULT_HEIGHT);
  glewInit();

}

void Window::enableGlFeatures(){
  // Enable textures
  glEnable(GL_TEXTURE_2D);
  // Enable depth testing
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  // Enable back face culling
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  setAmbientLight();
}

void Window::setAmbientLight(){
  // Set ambient and diffuse lighting
  GLfloat light_DiffAndAmb[4] = {1.f, 1.f, 1.f, 1.f};
  glLightfv(GL_LIGHT0, GL_AMBIENT, light_DiffAndAmb);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, light_DiffAndAmb);
}

void Window::setSize(int width, int height) {
  glViewport(0, 0, (GLsizei)width, (GLsizei)height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(60.0, (GLdouble)width/(GLdouble)height, 0.1f, 50.f);

  this->width = width;
  this->height = height;


  glMatrixMode(GL_MODELVIEW);
}


int Window::getWidth(){
  return this->width;
}

int Window::getHeight(){
  return this->height;
}
