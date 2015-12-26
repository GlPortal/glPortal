#ifndef GAME_STATE_HPP
#define GAME_STATE_HPP

#include "World.hpp"
#include "Game.hpp"
#include <stack>
#include <string>
#include <engine/Entity.hpp>
#include <engine/core/state/HandleGameFunction.hpp>
#include <SDL2/SDL_events.h>

namespace glPortal {

class GameState {
private:
  World *world;
public:
  void handleInput(Game &game);
  static void handleRunning(Game &game);
  static void handlePaused(Game &game);
  static void handleSplash(Game &game);
  static void handleMenu(Game &game);
  static void handleTerminal(Game &game);
  static void handleGameOverScreen(Game &game);
  static void handleWinScreen(Game &game);
};

} /* namespace glPortal */
#endif /* GAME_STATE_HPP */
