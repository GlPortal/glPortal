#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <SDL2/SDL_video.h>
#include <engine/Viewport.hpp>
#include <string>

namespace glPortal {

class Window : public Viewport {
public:
  void create(const char*);
  void setFullscreen();
  void swapBuffers();
  void getSize(int *width, int *height) const;
  void close();
  
private:
  void initGlew();
  int width;
  int height;
  SDL_Window *window;
  SDL_GLContext context;

  static const char *DEFAULT_TITLE;
  static const std::string GLEW_UNSUPPORTED_MESSAGE;
  static const std::string GLEW_INIT_ERROR_MESSAGE;
  static const int DEFAULT_WIDTH, DEFAULT_HEIGHT;
};

} /* namespace glPortal */

#endif /* WINDOW_HPP */
