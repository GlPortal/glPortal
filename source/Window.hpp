#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <SDL2/SDL_video.h>

namespace glPortal {

class Window {
public:
  void create(const char*, int, int, bool fullscreen);
  void setFullscreen();
  void swapBuffers();
  void getSize(int *width, int *height);
  void close();
  static float aspect;
private:
  SDL_Window *w;
  SDL_GLContext context;

};

} /* namespace glPortal */

#endif /* WINDOW_HPP */
