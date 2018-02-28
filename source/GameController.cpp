#include <glPortal/GameController.hpp>
#include <glPortal/Game.hpp>
#include <glPortal/WorldHelper.hpp>

#include <radix/core/event/EventDispatcher.hpp>
#include <radix/core/state/GameState.hpp>
#include <radix/env/Environment.hpp>
#include <radix/entities/Player.hpp>

namespace glPortal {

GameController::GameController(Game *game)
  : game(game),
  world(static_cast<World*>(game->getWorld())) {
  
  if (game->showSplash()) {
    handleSplash();
  }

  initObservers();
}

void GameController::initObservers() {
  radix::EventDispatcher &event = world->event;

  event.addObserverRaw(
    radix::InputSource::KeyReleasedEvent::Type, [this] (const radix::Event& event) {
      const int key = ((radix::InputSource::KeyReleasedEvent &) event).key;
      if (key == SDL_SCANCODE_RETURN) {
        if (this->state == Splash) {
          this->handleRunning();
        }
      }
      if (key == SDL_SCANCODE_ESCAPE) {
        if (this->state == Running) {
          this->handlePaused();
        } else if (this->state == Paused) {
          this->handleRunning();
        }
      }
    });

  event.addObserverRaw(
    radix::GameState::WinEvent::Type, [this](const radix::Event &event) {
      this->handleGameOver();
    });
  event.addObserverRaw(
    radix::InputSource::WindowCloseEvent::Type, [this](const radix::Event& event) {
      this->game->close();
    });

  World* _world = world;
  event.addObserverRaw(
    radix::InputSource::MouseButtonReleasedEvent::Type, [_world](const radix::Event &event) mutable {
      auto mouseEvent = (const radix::InputSource::MouseButtonReleasedEvent&) event;
      if (mouseEvent.button == radix::InputSource::MouseButton::Left) {
        WorldHelper::shootPortal(1, *_world);
      }
      if (mouseEvent.button == radix::InputSource::MouseButton::Right) {
        WorldHelper::shootPortal(2, *_world);
      }
    });
}

void GameController::handleRunning() {
  if (state == Splash) {
    game->getGameWorld()->removeScreen(game->getGameWorld()->splashScreen);
  } else if (state == Paused) {
    game->getGameWorld()->removeScreen(game->getGameWorld()->pauseScreen);
  }

  world->getPlayer().frozen = false;
  state = Running;
}

void GameController::handlePaused() {
  game->getGameWorld()->addScreen(game->getGameWorld()->pauseScreen);
  world->getPlayer().frozen = true;

  state = Paused;
}

void GameController::handleSplash() {
  world->getPlayer().frozen = true;
  game->getGameWorld()->addScreen(this->game->getGameWorld()->splashScreen);
  state = Splash;
  radix::Util::Log(radix::Info, "GameController") << "handledsplash";
}

void GameController::handleMenu() {}

void GameController::handleGameOver() {
  world->getPlayer().frozen = true;
  game->getGameWorld()->addScreen(game->getGameWorld()->gameOverScreen);
}

void GameController::handleWin() {}

void GameController::processInput() {
  game->getWindow().processEvents();
}

} /* namespace glPortal */
