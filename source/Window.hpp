#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <SDL2/SDL_video.h>
#include "engine/env/Environment.hpp"
#include <string>

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
  SDL_Window *window;
  SDL_GLContext context;
  ConfigFileParser * config;
  static const char* DEFAULT_TITLE;
  static const std::string GLEW_UNSUPPORTED_MESSAGE;
  static const std::string GLEW_INIT_ERROR_MESSAGE; 
};

} /* namespace glPortal */

#endif /* WINDOW_HPP */
