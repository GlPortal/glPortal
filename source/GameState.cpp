#include <glPortal/GameState.hpp>
#include <glPortal/Game.hpp>

#include <radix/data/screen/XmlScreenLoader.hpp>
#include <radix/env/Environment.hpp>
#include <radix/entities/Player.hpp>

namespace glPortal {

radix::EventDispatcher::CallbackHolder GameState::splashCallbackHolder;
radix::EventDispatcher::CallbackHolder GameState::winCallbackHolder;

void GameState::init(radix::BaseGame &game) {
  winCallbackHolder = game.getWorld()->event.addObserver(
      radix::GameState::WinEvent::Type, [&game](const radix::Event &event) {
        game.getWorld()->stateFunctionStack.push(&GameState::handleGameOverScreen);
      });

  winCallbackHolder.setStatic();
}

void GameState::handleRunning(radix::BaseGame &game) {
  radix::entities::Player &player = game.getWorld()->getPlayer();
  player.frozen = false;

  game.getWorld()->event.removeObserver(splashCallbackHolder);

  splashCallbackHolder = game.getWorld()->event.addObserver(
     radix::InputSource::KeyReleasedEvent::Type, [&player, &game](const radix::Event& event) {
       const int key = ((radix::InputSource::KeyReleasedEvent &) event).key;
       if (key == SDL_SCANCODE_ESCAPE) {
         player.frozen = true;
         game.getWorld()->stateFunctionStack.push(&GameState::handlePaused);
       }
     });

  if (!splashCallbackHolder.getStatic()) {
    splashCallbackHolder.setStatic();
  }
}

void GameState::handlePaused(radix::BaseGame &game) {
  radix::Screen &screen = radix::XmlScreenLoader::getScreen(radix::Environment::getDataDir()
                                                            + "/screens/title.xml");
  game.getGameWorld()->addScreen(screen);
  radix::entities::Player &player = game.getWorld()->getPlayer();

  game.getWorld()->event.removeObserver(splashCallbackHolder);

  splashCallbackHolder = game.getWorld()->event.addObserver
    (
     radix::InputSource::KeyReleasedEvent::Type, [&player, &game] (const radix::Event& event) {
       const int key = ((radix::InputSource::KeyReleasedEvent &) event).key;
       if (key == SDL_SCANCODE_ESCAPE) {
         player.frozen = false;
         game.getWorld()->stateFunctionStack.pop();
       }
     });
}

void GameState::handleSplash(radix::BaseGame &game) {
  radix::entities::Player &player = game.getWorld()->getPlayer();
  player.frozen = true;
  radix::Screen &screen =
    radix::XmlScreenLoader::getScreen(radix::Environment::getDataDir() + "/screens/test.xml");
  game.getGameWorld()->addScreen(screen);
  if (game.getWindow().isKeyDown(SDL_SCANCODE_RETURN)) {
    game.getWorld()->stateFunctionStack.pop();
  }
}

void GameState::handleMenu(radix::BaseGame &game) { }

void GameState::handleGameOverScreen(radix::BaseGame &game) {
  radix::entities::Player &player = game.getWorld()->getPlayer();
  player.frozen = true;
  radix::Screen &screen =
    radix::XmlScreenLoader::getScreen(radix::Environment::getDataDir() + "/screens/end.xml");
  game.getGameWorld()->addScreen(screen);
}

void GameState::handleWinScreen(radix::BaseGame &game) { }

} /* namespace glPortal */
