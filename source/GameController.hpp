#ifndef GAME_CONTROLLER_HPP
#define GAME_CONTROLLER_HPP

#include "Window.hpp"
#include "World.hpp"
#include <SDL2/SDL_events.h>

namespace glPortal {
class Game;
class GameController {
public:
  GameController(Game *game);
  void handleInput();
  void handleEvent(const SDL_Event &event);
private:
  World *world;
  Game *game;
  bool wasF5Down = false;
  bool wasTabDown = false;
};

} /* namespace glPortal */

#endif /* GAME_CONTROLLER_HPP */
