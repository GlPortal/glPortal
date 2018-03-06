#include <glPortal/input/InputManager.hpp>

#include <glPortal/Game.hpp>
#include <glPortal/GameController.hpp>
#include <glPortal/WorldHelper.hpp>

namespace glPortal {

InputManager::InputManager(Game *game)
  : radix::InputManager(game),
  gameController(game->getGameController()) {}

void InputManager::channelChanged(bool newValue, const int &id) {
  // do default actions as well as those which are glPortal-defined
  radix::InputManager::channelChanged(newValue, id);

  if (newValue) {
    switch (id) {
    case PLAYER_FIRE_PRIMARY : {
      WorldHelper::shootPortal(1, *game->getWorld());
      break;
    }
    case PLAYER_FIRE_SECONDARY : {
      WorldHelper::shootPortal(2, *game->getWorld());
      break;
    }
    case GAME_PAUSE : {
      gameController->togglePause();
      break;
    }
    case GAME_START : {
      gameController->handleRunning();
      break;
    }
    }
  }
}

} /* namespace glPortal */
