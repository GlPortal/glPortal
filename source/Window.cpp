#include "Window.hpp"
#include <GL/glew.h>
#include <iostream>
#include <cstdlib>

namespace glPortal {

void Window::create(const char* title, int width, int height) {
  SDL_Init(SDL_INIT_VIDEO);

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION,3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION,3);

  w = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                       width, height, SDL_WINDOW_OPENGL);


  //Create an OpenGL context associated with the window
  context = SDL_GL_CreateContext(w);

  glewExperimental = GL_TRUE;
  GLuint error = glewInit();

  if (error != GLEW_OK) {
    std::cout << "Glew is not OK" << std::endl;
    std::exit(1); // or handle the error in a nicer way
  }
  if (!GLEW_VERSION_2_1) { // check that the machine supports the 2.1 API.
    std::cout << "Machine does not support 2.1 API" << std::endl;
    std::exit(1); // or handle the error in a nicer way
  }

  /* Attributes */
  //Allow unbound framerate
  SDL_GL_SetSwapInterval(0);
  //Lock cursor in the middle of the screen
  SDL_SetRelativeMouseMode(SDL_TRUE);
}

void Window::swapBuffers() {
  SDL_GL_SwapWindow(w);
}

void Window::getSize(int *width, int *height) {
  SDL_GetWindowSize(w, width, height);
}

void Window::close() {
  SDL_GL_DeleteContext(context);
  SDL_DestroyWindow(w);
  w = NULL;

  SDL_Quit();
}

} /* namespace glPortal */
