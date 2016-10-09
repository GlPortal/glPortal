#include "GameController.hpp"
#include "Game.hpp"
#include <radix/env/Environment.hpp>

namespace glPortal {

GameController::GameController(Game *game) {
  this->game = game;
  this->world = static_cast<World*>(game->getWorld());
  this->gameState = std::make_unique<radix::GameState>();
}

void GameController::processInput() {
  game->getWindow().processEvents();
  if (game->getWindow().isKeyDown(SDL_SCANCODE_Q)) {
    game->close();
  }

  gameState->process(*game);
}

} /* namespace glPortal */
