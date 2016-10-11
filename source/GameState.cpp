#include <radix/screen/Screen.hpp>
#include <radix/screen/XMLScreenLoader.hpp>
#include <radix/env/Environment.hpp>
#include "GameState.hpp"

namespace glPortal {

void GameState::handleRunning(radix::BaseGame &game) {
}

void GameState::handlePaused(radix::BaseGame &game) { }

void GameState::handleSplash(radix::BaseGame &game) {
  radix::Screen &screen = radix::XMLScreenLoader::getScreen(radix::Environment::getDataDir() + "/screens/test.xml");
  game.addScreen(screen);
  if (game.getWindow().isKeyDown(SDL_SCANCODE_RETURN)) {
    game.getWorld()->stateFunctionStack.pop();
  }

}

void GameState::handleMenu(radix::BaseGame &game) { }

void GameState::handleGameOverScreen(radix::BaseGame &game) { }

void GameState::handleWinScreen(radix::BaseGame &game) { }

} /* namespace glPortal */
