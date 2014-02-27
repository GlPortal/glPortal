#ifndef __WINDOW_HPP
#define __WINDOW_HPP

#define DEFAULT_WIDTH 1680
#define DEFAULT_HEIGHT 1050

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/glext.h>
#include "Player.hpp"
#include "GameMap.hpp"
#include "engine/Box.hpp"
#include <vector>

using namespace glPortal::engine;

class Window {
public:
  void setup(int *, char **);
  void enableGlFeatures();
  int getWidth();
  int getHeight();
  void setSize(int, int);
private:
  void setAmbientLight();
  int width, height;
};

#endif
