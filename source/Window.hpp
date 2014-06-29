#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <SDL2/SDL.h>

namespace glPortal {

class Window {
  public:
    void create(const char*, int, int);
    void swapBuffers();
    void getSize(int *width, int *height);
    void close();
    void setFullscreen();
  private:
    SDL_Window *w;
    SDL_GLContext context;
};

} /* namespace glPortal */

#endif /* WINDOW_HPP */
