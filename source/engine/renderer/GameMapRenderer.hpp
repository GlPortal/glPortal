#ifndef __GAMEMAPRENDERER_HPP
#define __GAMEMAPRENDERER_HPP

#include "../../GameMap.hpp"

namespace glPortal {
  namespace engine{
    namespace renderer{
      class GameMapRenderer{
      private:
	GameMap* gameMap;
      public:
	GameMapRenderer(GameMap* gameMap);
	void render();
	void drawFromPortal(const Portal& portal);
      };
    }
  }
}

#endif
