#include "Window.hpp"

#include <GL/glew.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <cstdlib>
#include <iostream>

namespace glPortal {

float Window::aspect = 1;

void Window::create(const char* title, int width, int height, bool fullscreen) {
  SDL_Init(SDL_INIT_VIDEO);

  //Attributes
  SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

  int flags = SDL_WINDOW_OPENGL;
  if(fullscreen) {
    flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
  }
  //Create the window
  w = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
      width, height, flags);

  //Create an OpenGL context associated with the window
  context = SDL_GL_CreateContext(w);

  glewExperimental = GL_TRUE;
  GLuint error = glewInit();

  if (error != GLEW_OK) {
    std::cout << "Glew is not OK" << std::endl;
    printf("Error initializing GLEW! %s\n", glewGetErrorString(error));
    std::exit(1); // or handle the error in a nicer way
  }
  if (!GLEW_VERSION_2_1) { // check that the machine supports the 2.1 API.
    std::cout << "Machine does not support 2.1 API" << std::endl;
    std::exit(1); // or handle the error in a nicer way
  }

  //Set aspect ratio
  aspect = (float) width / height;
  //Allow unbound framerate
  SDL_GL_SetSwapInterval(0);
  //Lock cursor in the middle of the screen
  SDL_SetRelativeMouseMode(SDL_TRUE);
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
