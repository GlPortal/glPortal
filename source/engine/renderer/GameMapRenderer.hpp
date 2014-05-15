#ifndef __GAMEMAPRENDERER_HPP
#define __GAMEMAPRENDERER_HPP

#include "../../GameMap.hpp"
#include "../../Player.hpp"

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
        void renderPortals(Portal * portals, Player & player);
        void renderAvatar(Vector3f position);
        void drawCake();
        void drawLamp(int light);
      };
    }
  }
}

#endif
