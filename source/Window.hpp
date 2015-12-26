#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <memory>
#include <string>

#include <SDL2/SDL_video.h>

#include <Gwen/Controls/Canvas.h>
#include <Gwen/Skins/TexturedBase.h>

#include <engine/Viewport.hpp>

namespace glPortal {

class GWENRenderer;
class GWENInput;

class Window : public Viewport {
public:
  Window();
  ~Window();
  void create(const char*);
  void setFullscreen();
  void swapBuffers();
  void getSize(int *width, int *height) const;
  void close();

  void lockMouse();
  void unlockMouse();

  std::unique_ptr<GWENRenderer> gwenRenderer;
  std::unique_ptr<Gwen::Skin::TexturedBase> gwenSkin;
  std::unique_ptr<Gwen::Controls::Canvas> gwenCanvas;
  std::unique_ptr<GWENInput> gwenInput;
  
private:
  void initEpoxy();
  int width;
  int height;
  SDL_Window *window;
  SDL_GLContext context;

  static const char *DEFAULT_TITLE;
  static const int DEFAULT_WIDTH, DEFAULT_HEIGHT;
};

} /* namespace glPortal */

#endif /* WINDOW_HPP */
