#include "Window.hpp"
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>
#include "engine/Resources.hpp"
#include "Portal.hpp"
#include "engine/Environment.hpp"
#include <stdexcept>

void Window::setup(int *argc, char **argv) {
  ConfigFileParser config = Environment::getConfig();
  SDL_Init(SDL_INIT_EVERYTHING);
  
  try{
    this->width  =  config.getIntByKey("width");
    this->height =  config.getIntByKey("height");
  } catch (const std::invalid_argument& e){
    this->width = DEFAULT_WIDTH;
    this->height = DEFAULT_HEIGHT;
  }

  Uint32 windowConfigFlags = SDL_WINDOW_OPENGL;

  try{
    if(config.getStringByKey("fullscreen") == "no") {
      windowConfigFlags = windowConfigFlags | SDL_WINDOW_RESIZABLE | SDL_WINDOW_MAXIMIZED;
    } else {
      windowConfigFlags = windowConfigFlags | SDL_WINDOW_FULLSCREEN_DESKTOP;
    } 
  } catch (const std::invalid_argument& e) {
    windowConfigFlags = windowConfigFlags | SDL_WINDOW_FULLSCREEN_DESKTOP;
  }

  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

  SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS,  1);
  SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES,  4);

  w = SDL_CreateWindow(TITLE,
                       SDL_WINDOWPOS_UNDEFINED,
                       SDL_WINDOWPOS_UNDEFINED,
                       this->width,
                       this->height,
                       windowConfigFlags);

  if(w == NULL) {
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 0);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 0);

    w = SDL_CreateWindow(TITLE,
                         SDL_WINDOWPOS_UNDEFINED,
                         SDL_WINDOWPOS_UNDEFINED,
                         this->width,
                         this->height,
                         windowConfigFlags);
  }

  if(w == NULL) {
    printf("Could not create window: %s\n", SDL_GetError());
  }
  
  //Create an OpenGL context associated with the window
  SDL_GLContext gl_context = SDL_GL_CreateContext(w);
    
  //Initialize GLEW
  GLenum err = glewInit();
  if (GLEW_OK != err) {
    printf("Error: %s\n", glewGetErrorString(err));
  }
  
  SDL_GetWindowSize(w, &width, &height);
  updateViewport();
  
  //Set the pointer to be able to report continuous motion
  SDL_SetRelativeMouseMode(SDL_TRUE);
}

void Window::enableGlFeatures() {
  glEnable(GL_DEPTH_TEST);
  
  glDepthFunc(GL_LESS);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glShadeModel(GL_SMOOTH);
  setAmbientLight();
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_BLEND);
}

void Window::swapBuffer() {
  SDL_GL_SwapWindow(w);
}

void Window::setAmbientLight() {
  float diffuse[4] = {1.0f, 1.0f, 1.0f, 1.0f};
  float ambient[4] = {0.0f, 0.0f, 0.0f, 0.0f};
  
  glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
  glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
  glLightfv(GL_LIGHT0, GL_SPECULAR, diffuse);
  glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 0);
  glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.01);
  
  glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse);
  glLightfv(GL_LIGHT1, GL_AMBIENT, ambient);
  glLightfv(GL_LIGHT1, GL_SPECULAR, diffuse);
  glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 0);
  glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.01);
  
  glLightfv(GL_LIGHT2, GL_DIFFUSE, diffuse);
  glLightfv(GL_LIGHT2, GL_AMBIENT, ambient);
  glLightfv(GL_LIGHT2, GL_SPECULAR, diffuse);
  glLightf(GL_LIGHT2, GL_CONSTANT_ATTENUATION, 0);
  glLightf(GL_LIGHT2, GL_QUADRATIC_ATTENUATION, 0.01);
  
  glLightfv(GL_LIGHT3, GL_DIFFUSE, diffuse);
  
  glEnable(GL_LIGHT0);
  glEnable(GL_LIGHT1);
  glEnable(GL_LIGHT2);
  //glEnable(GL_LIGHT3);
}

void Window::setSize(int width, int height) {
  //Set the size of the window
  SDL_SetWindowSize(w, width, height);
  this->width = width;
  this->height = height;
  updateViewport();
}

void Window::updateViewport() {
  glViewport(0, 0, width, height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(60.0, (float) width/height, 0.1f, 50.f);
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
