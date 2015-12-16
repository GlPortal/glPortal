#include "World.hpp"
#include <stack>
#include <engine/Entity.hpp>
#include <SDL2/SDL_events.h>

namespace glPortal {
typedef std::function<void (Entity& player, const SDL_Event &event)> HandleEventFunction;
  
class PlayerState
{
private:
  World *world;
  std::stack<HandleEventFunction> stateFunctionStack;
public:
  PlayerState();
  void handleInput(Entity& player, const SDL_Event &event);
  static void handleInputStanding(Entity& player, const SDL_Event &event);
  static void handleInputRunning(Entity& player, const SDL_Event &event);
  static void handleInputJumping(Entity& player, const SDL_Event &event);
};
} /* namespace glPortal */
