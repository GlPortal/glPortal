#include "Window.hpp"

#include <epoxy/gl.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <cstdlib>
#include <stdexcept>
#include <iostream>
#include <engine/env/Environment.hpp>
#include <engine/env/System.hpp>

namespace glPortal {

const int Window::DEFAULT_WIDTH = 800;
const int Window::DEFAULT_HEIGHT = 600;
const char* Window::DEFAULT_TITLE = "GlPortal";

Window::Window() :
  width(0),
  height(0),
  window(nullptr) {
}

void Window::initEpoxy() {
  System::Log() << "GL " << epoxy_gl_version();
}

void Window::create(const char *title) {
  SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);

  SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

  Config &config = Environment::getConfig();

  if (config.getAntialiasLevel() > 0) {
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, config.getAntialiasLevel());
  }
  
  int flags = SDL_WINDOW_OPENGL;
  if (config.isFullscreen()) {
    flags |= SDL_WINDOW_BORDERLESS;
  }

  SDL_DisplayMode dispMode = {SDL_PIXELFORMAT_UNKNOWN, 0, 0, 0, 0};
  SDL_GetDesktopDisplayMode(0, &dispMode);

  int width = config.getWidth();
  int height = config.getHeight();

  if (width == 0) {
    width = dispMode.w;
  }
  if (height == 0) {
    height = dispMode.h;
  }

  window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
      width, height, flags);

  context = SDL_GL_CreateContext(window);

  initEpoxy();

  this->width = width;
  this->height = height;

  // Allows unbound framerate if vsync is disabled
  SDL_GL_SetSwapInterval(config.hasVsync() ? 1 : 0);

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
