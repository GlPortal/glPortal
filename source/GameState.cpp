#include <glPortal/GameState.hpp>
#include <glPortal/Game.hpp>

#include <radix/screen/XmlScreenLoader.hpp>
#include <radix/env/Environment.hpp>
#include <radix/component/Player.hpp>

namespace glPortal {

radix::EventDispatcher::CallbackHolder GameState::splashCallbackHolder;

void GameState::handleRunning(radix::BaseGame &game) {
  radix::Player &player = game.getWorld()->getPlayer().getComponent<radix::Player>();
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
}

void GameState::handlePaused(radix::BaseGame &game) {
  radix::Screen &screen = radix::XmlScreenLoader::getScreen(radix::Environment::getDataDir() + "/screens/test.xml");
  game.getGameWorld()->addScreen(screen);
  radix::Player &player = game.getWorld()->getPlayer().getComponent<radix::Player>();

  game.getWorld()->event.removeObserver(splashCallbackHolder);

  splashCallbackHolder = game.getWorld()->event.addObserver(
      radix::InputSource::KeyReleasedEvent::Type, [&player, &game] (const radix::Event& event) {
        const int key = ((radix::InputSource::KeyReleasedEvent &) event).key;
          if (key == SDL_SCANCODE_ESCAPE) {
            player.frozen = false;
            game.getWorld()->stateFunctionStack.pop();
          }
      });
}

void GameState::handleSplash(radix::BaseGame &game) {
  radix::Player &player = game.getWorld()->getPlayer().getComponent<radix::Player>();
  player.frozen = true;
  radix::Screen &screen = radix::XmlScreenLoader::getScreen(radix::Environment::getDataDir() + "/screens/test.xml");
  game.getGameWorld()->addScreen(screen);
  if (game.getWindow().isKeyDown(SDL_SCANCODE_RETURN)) {
    game.getWorld()->stateFunctionStack.pop();
  }
}

void GameState::handleMenu(radix::BaseGame &game) { }

void GameState::handleGameOverScreen(radix::BaseGame &game) { }

void GameState::handleWinScreen(radix::BaseGame &game) { }

} /* namespace glPortal */
