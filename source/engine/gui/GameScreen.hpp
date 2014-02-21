#ifndef __GAMESCREEN_HPP
#define __GAMESCREEN_HPP

#include <GL/glew.h>
#include <GL/glext.h>
#include <GL/freeglut.h>
#include "../../Window.hpp"
#include "Character.hpp"

namespace glPortal {
  namespace engine {
    namespace gui {
      class GameScreen {
      public:
	GameScreen(Window &window);
	void drawPauseScreen();
	void drawCrosshair();
	void drawGameOverScreen();
	void drawRespawnScreen();
	void drawContinueScreen();
	void drawTestTextScreen();
      private:
	Window window;
      };
    }
  }
}
#endif
