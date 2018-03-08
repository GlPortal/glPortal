#ifndef GLPORTAL_GAME_CONTROLLER_HPP
#define GLPORTAL_GAME_CONTROLLER_HPP

namespace glPortal {

class Game;

class GameController {
public:
  GameController() = delete;
  GameController(Game *game);
  void init();

  void processInput();
  void initObservers();

  void togglePause();
  void handleRunning();
  void handlePaused();
  void handleSplash();
  void handleMenu();
  void handleGameOver();
  void handleWin();

  enum State : signed char {
    Splash,
    Running,
    Paused,
    Menu,
    GameOver,
    Win
  };
  State getState() { return state; }

private:
  Game *game;
  State state;
  bool initialised;

};

} /* namespace glPortal */

#endif //GLPORTAL_GAME_CONTROLLER_HPP
