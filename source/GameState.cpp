#include "GameState.hpp"

namespace glPortal {

void GameState::handleRunning(radix::BaseGame &game) {
  game.getWorld()->hideScreen();
}

void GameState::handlePaused(radix::BaseGame &game) { }

void GameState::handleSplash(radix::BaseGame &game) {
  game.getWorld()->showScreen();
  if (game.getWindow().isKeyDown(SDL_SCANCODE_RETURN)) {
    game.getWorld()->hideScreen();
    game.getWorld()->stateFunctionStack.pop();
  }

}

void GameState::handleMenu(radix::BaseGame &game) { }

void GameState::handleGameOverScreen(radix::BaseGame &game) { }

void GameState::handleWinScreen(radix::BaseGame &game) { }

} /* namespace glPortal */
