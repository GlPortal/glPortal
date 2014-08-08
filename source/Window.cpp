#include "Window.hpp"

#include <GL/glew.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <cstdlib>
#include <iostream>
#include "engine/environment/ConfigFileParser.hpp"
#include "engine/environment/Environment.hpp"
#include <stdexcept>

namespace glPortal {

float Window::aspect = 1;

void Window::initGlew(){
  glewExperimental = GL_TRUE;
  GLuint error = glewInit();

  if (error != GLEW_OK) {
    printf("Error initializing GLEW! %s\n", glewGetErrorString(error));
    std::exit(1); 
  }
  if (!GLEW_VERSION_2_1) { 
    std::cout << "Your hardware does not support 2.1 API" << std::endl;
    std::exit(1); 
  }
}
  
void Window::create(const char* title, int width, int height, bool fullscreen) {
  SDL_Init(SDL_INIT_VIDEO);

  //Attributes
  SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

  std::string supersampling;
  try {
    supersampling = config->getStringByKey("supersampling");
  } catch (const std::invalid_argument& e) { }

  if (supersampling == "yes") {
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 16);
  }
  
  int flags = SDL_WINDOW_OPENGL;
  if(fullscreen) {
    flags |= SDL_WINDOW_BORDERLESS;
  }
  //Create the window
  w = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
      width, height, flags);

  //Create an OpenGL context associated with the window
  context = SDL_GL_CreateContext(w);

  this->initGlew();

  //Set aspect ratio
  aspect = (float) width / height;
  //Allow unbound framerate
  SDL_GL_SetSwapInterval(0);
  //Lock cursor in the middle of the screen
  SDL_SetRelativeMouseMode(SDL_TRUE);
}

void Window::createFromConfig(){
  config = Environment::getConfigPointer();
  int height, width;
  std::string fullscreen;
  try {
    height = config->getIntByKey("height");
    width = config->getIntByKey("width");
  } catch (const std::invalid_argument& e) {
    height = 600;
    width = 800;
  }
  try {
    fullscreen = config->getStringByKey("fullscreen");
  } catch (const std::invalid_argument& e) { }

  if (fullscreen == "yes") {
    this->create("GlPortal", width, height, true);
  } else {
    this->create("GlPortal", width, height, false);
  }
}
  
void Window::setFullscreen() {
  SDL_SetWindowFullscreen(w, SDL_WINDOW_FULLSCREEN);
}

void Window::swapBuffers() {
  SDL_GL_SwapWindow(w);
}

void Window::getSize(int* width, int* height) {
  SDL_GetWindowSize(w, width, height);
}

void Window::close() {
  SDL_GL_DeleteContext(context);
  SDL_DestroyWindow(w);
  w = NULL;

  SDL_Quit();
}

} /* namespace glPortal */
