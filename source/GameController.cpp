#include <glPortal/GameController.hpp>

#include <glPortal/Game.hpp>
#include <glPortal/WorldHelper.hpp>

#include <radix/core/event/EventDispatcher.hpp>
#include <radix/core/state/GameStateManager.hpp>
#include <radix/entities/Player.hpp>
#include <radix/input/InputManager.hpp>

namespace glPortal {

GameController::GameController(Game *game)
  : game(game),
  initialised(false) {
}

void GameController::init() {
  if (!initialised) {
    initialised = true;
    handleSplash();
  }

  initObservers();
}

void GameController::processInput() {
  game->getWindow().processEvents();
}

void GameController::initObservers() {
  radix::EventDispatcher &event = game->getWorld()->event;

  event.addObserverRaw(
    radix::GameStateManager::WinEvent::Type, [this](const radix::Event &event) {
      this->handleGameOver();
    });
  event.addObserverRaw(
    radix::InputSource::WindowCloseEvent::Type, [this](const radix::Event& event) {
      this->game->close();
    });
}

void GameController::togglePause() {
  if (state == Running) {
    handlePaused();
  } else {
    handleRunning();
  }
}

void GameController::handleRunning() {
  if (state == Splash) {
    game->getGameWorld()->removeScreen(game->getGameWorld()->splashScreen);
    game->getWorld()->getPlayer().frozen = false;
    state = Running;
  } else if (state == Paused) {
    game->getGameWorld()->removeScreen(game->getGameWorld()->pauseScreen);
    game->getWorld()->getPlayer().frozen = false;
    state = Running;
  }
}

void GameController::handlePaused() {
  if (state == Running) {
    game->getGameWorld()->addScreen(game->getGameWorld()->pauseScreen);
    game->getWorld()->getPlayer().frozen = true;
    state = Paused;
  }
}

void GameController::handleSplash() {
  game->getWorld()->getPlayer().frozen = true;
  game->getGameWorld()->addScreen(this->game->getGameWorld()->splashScreen);
  state = Splash;
}

void GameController::handleMenu() {}

void GameController::handleGameOver() {
  game->getWorld()->getPlayer().frozen = true;
  game->getGameWorld()->addScreen(game->getGameWorld()->gameOverScreen);
}

void GameController::handleWin() {}

} /* namespace glPortal */
