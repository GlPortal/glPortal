#include "World.hpp"
#include "Game.hpp"
#include <stack>
#include <engine/Entity.hpp>
#include <SDL2/SDL_events.h>

namespace glPortal {
typedef std::function<void (Game& game, const SDL_Event &event)> HandleEventGameFunction;
  
class GameState
{
private:
  World *world;
  std::stack<HandleEventGameFunction> stateFunctionStack;
public:
  GameState();
  void handleInput(Game& game, const SDL_Event &event);
  static void handleInputRunning(Game& game, const SDL_Event &event);
  static void handleInputPaused(Game& game, const SDL_Event &event);
  static void handleInputSplash(Game& game, const SDL_Event &event);
  static void handleInputMenu(Game& game, const SDL_Event &event);
  static void handleInputGameOverScreen(Game& game, const SDL_Event &event);
  static void handleInputWinScreen(Game& game, const SDL_Event &event);
};
} /* namespace glPortal */
