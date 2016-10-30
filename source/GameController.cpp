#include "GameController.hpp"
#include "Game.hpp"
#include "GameState.hpp"

#include <radix/env/Environment.hpp>
#include <radix/component/Player.hpp>

namespace glPortal {

GameController::GameController(Game *game) {
  this->game = game;
  this->world = static_cast<World*>(game->getWorld());
  this->gameState = std::make_unique<radix::GameState>();
  this->splashOffCallbackHolder = game->getWorld()->event.addObserver(
      radix::InputSource::KeyReleasedEvent::Type, [this] (const radix::Event& event) {
        this->passed = false;
        if (this->paused) {
          const int key = ((radix::InputSource::KeyReleasedEvent &) event).key;
          if (key == SDL_SCANCODE_ESCAPE) {
            this->paused = false;
            radix::Player &player = this->game->getWorld()->getPlayer().getComponent<radix::Player>();
            player.frozen = false;
            this->game->getWorld()->stateFunctionStack.pop();
            this->passed = true;
          }
        }
      });
  this->splashOnCallbackHolder = game->getWorld()->event.addObserver(
      radix::InputSource::KeyReleasedEvent::Type, [this](const radix::Event& event) {
        if (not this->paused) {
          if (not passed) {
            const int key = ((radix::InputSource::KeyReleasedEvent &) event).key;
            if (key == SDL_SCANCODE_ESCAPE) {
              this->paused = true;
              radix::Player &player = this->game->getWorld()->getPlayer().getComponent<radix::Player>();
              player.frozen = true;
              this->game->getWorld()->stateFunctionStack.push(&GameState::handlePaused);
            }
          }
        }
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
