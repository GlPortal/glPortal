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

const int Window::DEFAULT_WIDTH = 800;
const int Window::DEFAULT_HEIGHT = 600;
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

void Window::create(const char *title, int width, int height, bool fullscreen) {
  SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER);

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

  SDL_DisplayMode dispMode = {SDL_PIXELFORMAT_UNKNOWN, 0, 0, 0, 0};
  SDL_GetDesktopDisplayMode(0, &dispMode);
  if (width == -1) {
    width = dispMode.w;
  }
  if (height == -1) {
    height = dispMode.h;
  }

  window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
      width, height, flags);

  context = SDL_GL_CreateContext(window);

  this->initGlew();

  this->width = width;
  this->height = height;

  bool vsync = true;
  try {
    std::string vsyncOpt = config->getStringByKey(Config::VSYNC);
    vsync = (vsyncOpt == Config::TRUE);
  } catch (const std::invalid_argument& e) { }
  // Allows unbound framerate if vsync is disabled
  SDL_GL_SetSwapInterval(vsync ? 1 : 0);

  //Lock cursor in the middle of the screen
  SDL_SetRelativeMouseMode(SDL_TRUE);
}

void Window::createFromConfig(){
  config = Environment::getConfigPointer();
  int height, width;
  bool fullscreen = false;
  try {
    fullscreen = (config->getStringByKey(Config::FULLSCREEN) == Config::TRUE);
  } catch (const std::invalid_argument &e) { }

  try {
    std::string widthConf = config->getStringByKey(Config::WIDTH);
    if (widthConf == "auto") {
      width = -1;
    } else {
      width = std::stoi(widthConf);
    }
  } catch (const std::invalid_argument &e) {
    width = DEFAULT_WIDTH;
  }
  try {
    std::string heightConf = config->getStringByKey(Config::HEIGHT);
    if (heightConf == "auto") {
      height = -1;
    } else {
      height = std::stoi(heightConf);
    }
  } catch (const std::invalid_argument &e) {
    height = DEFAULT_HEIGHT;
  }

  this->create(DEFAULT_TITLE, width, height, fullscreen);
}

void Window::setFullscreen() {
  SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
}

void Window::swapBuffers() {
  SDL_GL_SwapWindow(window);
}

void Window::getSize(int *width, int *height) const {
  SDL_GetWindowSize(window, width, height);
}

void Window::close() {
  SDL_GL_DeleteContext(context);
  SDL_DestroyWindow(window);
  window = nullptr;

  SDL_Quit();
}

} /* namespace glPortal */
