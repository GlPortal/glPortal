#ifndef GAME_CONTROLLER_HPP
#define GAME_CONTROLLER_HPP

#include <SDL2/SDL_events.h>
#include <engine/core/state/PlayerState.hpp>
#include <engine/core/state/GameState.hpp>

namespace glPortal {
class Game;
class World;
class GameController {
public:
  GameController(Game *game);
  void handleInput();
  void handleEvent(const SDL_Event &event);
private:
  World *world;
  Game *game;
  std::unique_ptr<PlayerState> playerState;
  std::unique_ptr<GameState> gameState;
};

} /* namespace glPortal */

#endif /* GAME_CONTROLLER_HPP */
