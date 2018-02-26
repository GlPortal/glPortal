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
  enum State : int8_t {
    Splash,
    Running,
    Paused,
    Menu,
    GameOver,
    Win
  };
  State state;

  GameController() = delete;
  GameController(Game *game);

  void initObservers();
  void processInput();

private:
  void handleRunning();
  void handlePaused();
  void handleSplash();
  void handleMenu();
  void handleGameOver();
  void handleWin();

  Game *game;
  World *world;
};

} /* namespace glPortal */

#endif //GLPORTAL_GAMECONTROLLER_HPP
