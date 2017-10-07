#include <glPortal/GameController.hpp>
#include <glPortal/Game.hpp>

#include <radix/env/Environment.hpp>
#include <radix/entities/Player.hpp>
#include <glPortal/GameState.hpp>
#include <glPortal/WorldHelper.hpp>

namespace glPortal {

GameController::GameController(Game *game) {
  this->game = game;
  this->world = static_cast<World*>(game->getWorld());
  this->gameState = std::make_unique<radix::GameState>();
  GameState::init(*game);
  initObservers();
}

void GameController::initObservers() {
  this->closeWindowHolder = world->event.addObserver(
      radix::InputSource::WindowCloseEvent::Type, [this](const radix::Event& event) {
        this->game->close();
    });
  this->mouseHolder = world->event.addObserver(
      radix::InputSource::MouseButtonReleasedEvent::Type, [this](const radix::Event &event) {
        auto mouseEvent = (const radix::InputSource::MouseButtonReleasedEvent&) event;
        if (mouseEvent.button == radix::InputSource::MouseButton::Left) {
            WorldHelper::shootPortal(1, *world);
        }
        if (mouseEvent.button == radix::InputSource::MouseButton::Right) {
            WorldHelper::shootPortal(2, *world);
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
