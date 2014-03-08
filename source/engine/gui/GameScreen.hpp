#ifndef __GAMESCREEN_HPP
#define __GAMESCREEN_HPP

#include <GL/glew.h>
#include <GL/glext.h>
#include "../../Window.hpp"
#include "Character.hpp"
#include "Font.hpp"
#include "../tools/Timer.hpp"
#include <string>
using namespace glPortal::engine::tools;

namespace glPortal {
  namespace engine {
    namespace gui {
      class GameScreen {
      public:
	      GameScreen(Window &window);
	      void beginOverlay();
	      void endOverlay();
	      void drawPauseScreen();
	      void drawCrosshair();
	      void drawPortalEgg();
	      void drawJetPackEggs();
	      void drawGameOverScreen();
	      void drawRespawnScreen();
	      void drawContinueScreen();
	      void drawTestTextScreen();
	      void drawCreditsScreen();
        void drawTimer(std::string timeString);
      private:
        Timer gameTimer;
	      Window window;
      };
    }
  }
}
#endif
