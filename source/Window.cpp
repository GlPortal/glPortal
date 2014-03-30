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
  updateViewport();
}

void Window::updateViewport() {
  glViewport(0, 0, width, height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(60.0, (float) width/height, 0.01f, 50.f);
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
