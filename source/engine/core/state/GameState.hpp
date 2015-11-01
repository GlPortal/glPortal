#include "World.hpp"
#include "Game.hpp"
#include <stack>
#include <engine/Entity.hpp>
#include <SDL2/SDL_events.h>

namespace glPortal {
typedef std::function<int (Game &game)> HandleGameFunction;
  
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
