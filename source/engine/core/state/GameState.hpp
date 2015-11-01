#ifndef GAME_STATE_HPP
#define GAME_STATE_HPP

#include "World.hpp"
#include "Game.hpp"
#include <stack>
#include <engine/Entity.hpp>
#include <engine/core/state/HandleGameFunction.hpp>
#include <SDL2/SDL_events.h>

namespace glPortal {
  
class GameState
{
private:
  World *world;
  std::stack<HandleGameFunction> stateFunctionStack;
public:
  static const int POP_STACK;
  static const int DO_NOTHING;
  GameState();
  void handleInput(Game &game);
  static int handleRunning(Game &game);
  static void handlePaused(Game &game);
  static int handleSplash(Game &game);
  static void handleMenu(Game &game);
  static void handleGameOverScreen(Game &game);
  static void handleWinScreen(Game &game);
};
} /* namespace glPortal */
#endif /* GAME_STATE_HPP */
