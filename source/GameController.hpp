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

  bool paused = false;
private:
  Game *game;
  World *world;

  std::unique_ptr<radix::GameState> gameState;
  radix::EventDispatcher::CallbackHolder splashOnCallbackHolder;
  radix::EventDispatcher::CallbackHolder splashOffCallbackHolder;

  bool passed = false;
};

} /* namespace glPortal */

#endif //GLPORTAL_GAMECONTROLLER_HPP
