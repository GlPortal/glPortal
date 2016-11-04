#include <glPortal/GameController.hpp>
#include <glPortal/Game.hpp>

#include <radix/env/Environment.hpp>
#include <radix/component/Player.hpp>

namespace glPortal {

GameController::GameController(Game *game) {
  this->game = game;
  this->world = static_cast<World*>(game->getWorld());
  this->gameState = std::make_unique<radix::GameState>();
  this->closeWindowHolder = game->getWorld()->event.addObserver(
      radix::InputSource::WindowCloseEvent::Type, [game](const radix::Event& event) {
        game->close();
  });
}

void GameController::processInput() {
  game->getWindow().processEvents();
  if (game->getWindow().isKeyDown(SDL_SCANCODE_Q)) {
    game->close();
  }

  gameState->handleInput(*game);
}

} /* namespace glPortal */
