#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <SDL2/SDL_video.h>
#include "engine/env/Environment.hpp"

namespace glPortal {

class Window {
public:
  void create(const char*, int, int, bool fullscreen);
  void createFromConfig();
  void setFullscreen();
  void swapBuffers();
  void getSize(int *width, int *height);
  void close();
  static int width;
  static int height;
private:
  void initGlew();
  SDL_Window *w;
  SDL_GLContext context;
  ConfigFileParser * config;
};

} /* namespace glPortal */

#endif /* WINDOW_HPP */
