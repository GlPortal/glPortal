#include "World.hpp"
#include "Game.hpp"
#include <stack>
#include <engine/Entity.hpp>
#include <SDL2/SDL_events.h>

namespace glPortal {
typedef std::function<void (Game &game)> HandleGameFunction;
  
class GameState
{
private:
  World *world;
  std::stack<HandleGameFunction> stateFunctionStack;
public:
  GameState();
  void handleInput(Game &game);
  static void handleRunning(Game &game);
  static void handlePaused(Game &game);
  static void handleSplash(Game &game);
  static void handleMenu(Game &game);
  static void handleGameOverScreen(Game &game);
  static void handleWinScreen(Game &game);
};
} /* namespace glPortal */
