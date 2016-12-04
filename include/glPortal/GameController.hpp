#ifndef GLPORTAL_GAMECONTROLLER_HPP
#define GLPORTAL_GAMECONTROLLER_HPP

#include <radix/core/state/GameState.hpp>
#include <radix/core/event/EventDispatcher.hpp>

namespace radix {
  class ScreenRenderer;
} /* namespace radix */

namespace glPortal {

class Game;
class World;

class GameController {
public:
  GameController(Game *game);
  void processInput();

private:
  void initObservers();

  Game *game;
  World *world;

  radix::EventDispatcher::CallbackHolder closeWindowHolder;
  std::unique_ptr<radix::GameState> gameState;
};

} /* namespace glPortal */

#endif //GLPORTAL_GAMECONTROLLER_HPP
