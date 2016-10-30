#include "GameState.hpp"
#include "Game.hpp"

#include <radix/screen/XmlScreenLoader.hpp>
#include <radix/env/Environment.hpp>
#include <radix/component/Player.hpp>

namespace glPortal {

void GameState::handleRunning(radix::BaseGame &game) {
  radix::Player &player = game.getWorld()->getPlayer().getComponent<radix::Player>();
  player.frozen = false;
  if (game.getWindow().isKeyDown(SDL_SCANCODE_Q)) {
    game.close();
  }
}

void GameState::handlePaused(radix::BaseGame &game) {
  radix::Screen &screen = radix::XmlScreenLoader::getScreen(radix::Environment::getDataDir() + "/screens/test.xml");
  game.getGameWorld()->addScreen(screen);
}

void GameState::handleSplash(radix::BaseGame &game) {
  radix::Player &player = game.getWorld()->getPlayer().getComponent<radix::Player>();
  player.frozen = true;
  radix::Screen &screen = radix::XmlScreenLoader::getScreen(radix::Environment::getDataDir() + "/screens/test.xml");
  game.getGameWorld()->addScreen(screen);
  if (game.getWindow().isKeyDown(SDL_SCANCODE_RETURN)) {
    game.getWorld()->stateFunctionStack.pop();
  }
  if (game.getWindow().isKeyDown(SDL_SCANCODE_Q)) {
    game.close();
  }
}

void GameState::handleMenu(radix::BaseGame &game) { }

void GameState::handleGameOverScreen(radix::BaseGame &game) { }

void GameState::handleWinScreen(radix::BaseGame &game) { }

} /* namespace glPortal */
