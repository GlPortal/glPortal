#include "Window.hpp"

#include <GL/glew.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <cstdlib>
#include "engine/env/ConfigFileParser.hpp"
#include "engine/env/Config.hpp"
#include "engine/env/Environment.hpp"
#include <stdexcept>
#include <iostream>
namespace glPortal {

int Window::width = 800;
int Window::height = 600;
  const char* Window::DEFAULT_TITLE = "GlPortal";
  const std::string Window::GLEW_UNSUPPORTED_MESSAGE =
    "Your hardware does not support GLEW 2.1 API\n";
  const std::string Window::GLEW_INIT_ERROR_MESSAGE =
    "Error initializing GLEW.";

void Window::initGlew(){
  glewExperimental = GL_TRUE;
  GLuint glewInitReturnValue = glewInit();

  if (glewInitReturnValue != GLEW_OK) {
    std::cout << GLEW_INIT_ERROR_MESSAGE << " " <<  glewGetErrorString(glewInitReturnValue) << endl;
    std::exit(1);
  }
  if (not GLEW_VERSION_2_1) {
    std::cout << GLEW_UNSUPPORTED_MESSAGE << endl;
    std::exit(1);
  }
}

void Window::create(const char* title, int width, int height, bool fullscreen) {
  SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);

  SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

  std::string supersampling;
  try {
    supersampling = config->getStringByKey(Config::SUPERSAMPLING);
  } catch (const std::invalid_argument& e) { }

  if (supersampling == Config::TRUE) {
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);
  }
  
  int flags = SDL_WINDOW_OPENGL;
  if (fullscreen) {
    flags |= SDL_WINDOW_BORDERLESS;
  }

  window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
      width, height, flags);

  context = SDL_GL_CreateContext(window);

  this->initGlew();

  this->width = width;
  this->height = height;

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
    height = config->getIntByKey(Config::HEIGHT);
    width = config->getIntByKey(Config::WIDTH);
  } catch (const std::invalid_argument& e) {
    height = 600;
    width = 800;
  }
  
  try {
    fullscreen = config->getStringByKey(Config::FULLSCREEN);
  } catch (const std::invalid_argument& e) { }

  bool settingIsFullscreen = false;
  if (fullscreen == Config::TRUE) {
    settingIsFullscreen = true;
  } else {
    settingIsFullscreen = false;
  }
  this->create(DEFAULT_TITLE, width, height, settingIsFullscreen);
}

void Window::setFullscreen() {
  SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
}

void Window::swapBuffers() {
  SDL_GL_SwapWindow(window);
}

void Window::getSize(int* width, int* height) {
  SDL_GetWindowSize(window, width, height);
}

void Window::close() {
  SDL_GL_DeleteContext(context);
  SDL_DestroyWindow(window);
  window = NULL;

  SDL_Quit();
}

} /* namespace glPortal */
