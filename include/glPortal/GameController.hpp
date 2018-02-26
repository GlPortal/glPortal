#ifndef GLPORTAL_GAME_CONTROLLER_HPP
#define GLPORTAL_GAME_CONTROLLER_HPP

namespace glPortal {

class Game;
class World;

class GameController {
public:
  enum State : char {
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

#endif //GLPORTAL_GAME_CONTROLLER_HPP
