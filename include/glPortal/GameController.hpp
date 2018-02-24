#ifndef GLPORTAL_GAMECONTROLLER_HPP
#define GLPORTAL_GAMECONTROLLER_HPP

#include <radix/core/state/GameStateManager.hpp>
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
  radix::EventDispatcher::CallbackHolder mouseHolder;
  std::unique_ptr<radix::GameStateManager> gameStateManager;
};

} /* namespace glPortal */

#endif //GLPORTAL_GAMECONTROLLER_HPP
