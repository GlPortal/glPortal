#ifndef __GAMESCREEN_HPP
#define __GAMESCREEN_HPP

#include <GL/glew.h>
#include <GL/glext.h>
#include <GL/freeglut.h>
#include "Window.hpp"

class GameScreen {
public:
  GameScreen(Window &window);
  void drawPauseScreen();
  void drawCrosshair();
  void drawGameOverScreen();
  void drawRespawnScreen();
  void drawContinueScreen();
private:
  Window window;
};

#endif
