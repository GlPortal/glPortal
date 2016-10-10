#include "GameState.hpp"
#include <radix/component/Player.hpp>

namespace glPortal {

void GameState::handleRunning(radix::BaseGame &game) {
  radix::Player &player = game.getWorld()->getPlayer().getComponent<radix::Player>();
  player.frozen = false;
  game.getGameWorld()->hideScreen();
  if (game.getWindow().isKeyDown(SDL_SCANCODE_Q)) {
    game.close();
  }
  if (game.getWindow().isKeyDown(SDL_SCANCODE_ESCAPE)) {
    player.frozen = true;
    game.getWorld()->stateFunctionStack.push(&GameState::handlePaused);
    game.getGameWorld()->showScreen();
  }
}

void GameState::handlePaused(radix::BaseGame &game) {
  radix::Player &player = game.getWorld()->getPlayer().getComponent<radix::Player>();
  if (game.getWindow().isKeyDown(SDL_SCANCODE_ESCAPE)) {
    player.frozen = false;
    game.getWorld()->stateFunctionStack.pop();
    game.getGameWorld()->hideScreen();
  }
}

void GameState::handleSplash(radix::BaseGame &game) {
  game.getGameWorld()->showScreen();
  radix::Player &player = game.getWorld()->getPlayer().getComponent<radix::Player>();
  player.frozen = true;
  if (game.getWindow().isKeyDown(SDL_SCANCODE_RETURN)) {
    game.getGameWorld()->hideScreen();
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
