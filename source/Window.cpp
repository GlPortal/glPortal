#include "Window.hpp"
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>
#include "engine/Resources.hpp"
#include "Portal.hpp"

void Window::setup(int *argc, char **argv) {
  //Initialize SDL
  SDL_Init(SDL_INIT_EVERYTHING);
  w = SDL_CreateWindow(TITLE,
                       SDL_WINDOWPOS_UNDEFINED,
                       SDL_WINDOWPOS_UNDEFINED,
                       DEFAULT_WIDTH,
                       DEFAULT_HEIGHT,
                       SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN_DESKTOP);
  
  if(w == NULL) {
    printf("Could not create window: %s\n", SDL_GetError());
  }
  
  //Create an OpenGL context associated with the window
  SDL_GLContext gl_context = SDL_GL_CreateContext(w);
  
  //Set double buffering
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  
  //Initialize GLEW
  GLenum err = glewInit();
  if (GLEW_OK != err) {
    printf("Error: %s\n", glewGetErrorString(err));
  }
  
  //Get the size of the window and store it
  SDL_GetWindowSize(w, &width, &height);
  
  setSize(width, height);
  
  //Set the pointer to be able to report continuous motion
  SDL_SetRelativeMouseMode(SDL_TRUE);
}

void Window::enableGlFeatures() {
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glEnable(GL_LINE_SMOOTH);
  glEnable(GL_POLYGON_SMOOTH);
  glEnable(GL_MULTISAMPLE_ARB);
  glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
  glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
  glDepthFunc(GL_LESS);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  setAmbientLight();
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Window::swapBuffer() {
  SDL_GL_SwapWindow(w);
}

void Window::setAmbientLight(){
  GLfloat light_DiffAndAmb[4] = {1.f, 1.f, 1.f, 1.f};
  glLightfv(GL_LIGHT0, GL_AMBIENT, light_DiffAndAmb);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, light_DiffAndAmb);
}

void Window::setSize(int width, int height) {
  //Set the size of the window
  SDL_SetWindowSize(w, width, height);
  this->width = width;
  this->height = height;
  
  //Fix the OpenGL viewport
  glViewport(0, 0, (GLsizei)width, (GLsizei)height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(60.0, (GLdouble)width/(GLdouble)height, 0.1f, 50.f);
  glMatrixMode(GL_MODELVIEW);
}

void Window::close() {
    //Destroy window
    SDL_DestroyWindow(w);
    w = NULL;
    
    //Quit SDL
    SDL_Quit();
}

int Window::getWidth(){
  return this->width;
}

int Window::getHeight(){
  return this->height;
}
