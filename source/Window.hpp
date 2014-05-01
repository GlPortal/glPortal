#ifndef __WINDOW_HPP
#define __WINDOW_HPP

#define TITLE "GL Portal"
#define DEFAULT_WIDTH 1024
#define DEFAULT_HEIGHT 768

#include <GL/glew.h>
#include <GL/glext.h>
#include <SDL2/SDL.h>
#include "Player.hpp"
#include "GameMap.hpp"
#include "engine/Box.hpp"
#include <vector>

using namespace glPortal::engine;

class Window {
public:
  void setup(int argc, char * argv[]);
  void enableGlFeatures();
  void swapBuffer();
  int getWidth();
  int getHeight();
  void setSize(int, int);
  void updateViewport();
  void close();
private:
  void setAmbientLight();
  int width, height;
  SDL_Window *w;
};

#endif
