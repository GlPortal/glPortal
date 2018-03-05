#ifndef GLPORTAL_INPUT_MANAGER_HPP
#define GLPORTAL_INPUT_MANAGER_HPP

#include <radix/input/InputManager.hpp>

namespace glPortal {

class GameController;
class Game;

class InputManager : public radix::InputManager {
public:
  InputManager() = delete;
  InputManager(Game *game);

  void channelChanged(bool newValue, const int &id) override;

private:
  GameController *gameController;

};

} /* namespace glPortal */

#endif /* GLPORTAL_INPUT_MANAGER_HPP */
