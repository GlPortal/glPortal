#include "Window.hpp"

#include <GL/glew.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <cstdlib>
#include <engine/env/Config.hpp>
#include <stdexcept>
#include <iostream>
#include <engine/env/System.hpp>

namespace glPortal {

const int Window::DEFAULT_WIDTH = 800;
const int Window::DEFAULT_HEIGHT = 600;
const char* Window::DEFAULT_TITLE = "GlPortal";
const std::string Window::GLEW_UNSUPPORTED_MESSAGE =
  "Your hardware does not support GLEW 2.1 API\n";
const std::string Window::GLEW_INIT_ERROR_MESSAGE =
  "Error initializing GLEW.";

void Window::initGlew() {
  glewExperimental = GL_TRUE;
  GLuint glewInitReturnValue = glewInit();

  if (glewInitReturnValue != GLEW_OK) {
    System::Log() << GLEW_INIT_ERROR_MESSAGE << " " <<  glewGetErrorString(glewInitReturnValue);
    std::exit(1);
  }
  if (not GLEW_VERSION_2_1) {
    System::Log() << GLEW_UNSUPPORTED_MESSAGE;
    std::exit(1);
  }
}

void Window::create(const char *title) {
  SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER);

  SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

  if (Config::hasAntialiasing()) {
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);
  }
  
  int flags = SDL_WINDOW_OPENGL;
  if (Config::isFullscreen()) {
    flags |= SDL_WINDOW_BORDERLESS;
  }

  SDL_DisplayMode dispMode = {SDL_PIXELFORMAT_UNKNOWN, 0, 0, 0, 0};
  SDL_GetDesktopDisplayMode(0, &dispMode);
  int width = Config::getWidth();
  int height = Config::getHeight();
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

  // Allows unbound framerate if vsync is disabled
  SDL_GL_SetSwapInterval(Config::hasVsync() ? 1 : 0);

  // Lock cursor in the middle of the screen
  lockMouse();
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

void Window::lockMouse() {
  SDL_SetRelativeMouseMode(SDL_TRUE);
}

void Window::unlockMouse() {
  SDL_SetRelativeMouseMode(SDL_FALSE);
}

} /* namespace glPortal */
