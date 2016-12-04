#ifndef GLPORTAL_GAMESTATE_HPP
#define GLPORTAL_GAMESTATE_HPP

#include <radix/BaseGame.hpp>

namespace glPortal {

class GameState {
public:
  static void handleRunning(radix::BaseGame &game);

  static void handlePaused(radix::BaseGame &game);

  static void handleSplash(radix::BaseGame &game);

  static void handleMenu(radix::BaseGame &game);

  static void handleGameOverScreen(radix::BaseGame &game);

  static void handleWinScreen(radix::BaseGame &game);

  static radix::EventDispatcher::CallbackHolder splashCallbackHolder;
};

} /* namespace glPortal */


#endif //GLPORTAL_GAMESTATE_HPP
