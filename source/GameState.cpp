#include <radix/screen/Screen.hpp>
#include <radix/screen/XMLScreenLoader.hpp>
#include <radix/env/Environment.hpp>
#include "GameState.hpp"
#include <radix/component/Player.hpp>

namespace glPortal {

void GameState::handleRunning(radix::BaseGame &game) {
  radix::Player &player = game.getWorld()->getPlayer().getComponent<radix::Player>();
  player.frozen = false;
  if (game.getWindow().isKeyDown(SDL_SCANCODE_Q)) {
    game.close();
  }
  if (game.getWindow().isKeyDown(SDL_SCANCODE_ESCAPE)) {
    player.frozen = true;
    game.getWorld()->stateFunctionStack.push(&GameState::handlePaused);
  }
}

void GameState::handlePaused(radix::BaseGame &game) {
  radix::Screen &screen = radix::XMLScreenLoader::getScreen(radix::Environment::getDataDir() + "/screens/test.xml");
  game.getGameWorld()->addScreen(screen);
  radix::Player &player = game.getWorld()->getPlayer().getComponent<radix::Player>();
  if (game.getWindow().isKeyDown(SDL_SCANCODE_ESCAPE)) {
    player.frozen = false;
    game.getWorld()->stateFunctionStack.pop();
  }
}

void GameState::handleSplash(radix::BaseGame &game) {
  radix::Player &player = game.getWorld()->getPlayer().getComponent<radix::Player>();
  player.frozen = true;
  radix::Screen &screen = radix::XMLScreenLoader::getScreen(radix::Environment::getDataDir() + "/screens/test.xml");
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
