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
    this->width =  config.getConfigIntValueByKey("width");
    this->height =  config.getConfigIntValueByKey("height");
  } catch (const std::invalid_argument& e){
    this->width = DEFAULT_WIDTH;
    this->height = DEFAULT_HEIGHT;
  }

  Uint32 windowConfigFlags = SDL_WINDOW_OPENGL;

  try{
    if(config.getConfigValueByKey("fullscreen") == "no") {
      windowConfigFlags = windowConfigFlags | SDL_WINDOW_RESIZABLE | SDL_WINDOW_MAXIMIZED;
    } else {
      windowConfigFlags = windowConfigFlags | SDL_WINDOW_FULLSCREEN_DESKTOP;
    } 
  } catch (const std::invalid_argument& e) {
    windowConfigFlags = windowConfigFlags | SDL_WINDOW_FULLSCREEN_DESKTOP;
  }

  int amountOfMultiSampleBuffers = 0;
  if(SDL_GL_GetAttribute(SDL_GL_MULTISAMPLEBUFFERS, &amountOfMultiSampleBuffers) == 0) {
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS,  1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES,  16);
  }

  w = SDL_CreateWindow(TITLE,
                       SDL_WINDOWPOS_UNDEFINED,
                       SDL_WINDOWPOS_UNDEFINED,
                       this->width,
                       this->height,
                       windowConfigFlags);

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
  
  //Set the pointer to be able to report continuous motion
  SDL_SetRelativeMouseMode(SDL_TRUE);
}

void Window::enableGlFeatures(){
  glEnable(GL_TEXTURE_2D);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  setAmbientLight();
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_BLEND);
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
