#include "Game.hpp"
#include "GameState.hpp"

#include <iostream>

using namespace radix;

namespace glPortal {

Game::Game() {
  windowTitle = "GlPortal";
  defaultMap = "/maps/n1.xml";
}

void Game::initHook() {
  world.stateFunctionStack.push(&GameState::handleRunning);
  world.stateFunctionStack.push(&GameState::handleSplash);
  gameController = std::make_unique<GameController>(this);
  gameRenderer = std::make_unique<GameRenderer>(static_cast<glPortal::World&>(world), *renderer.get());
  uiRenderer = std::make_unique<UiRenderer>(static_cast<glPortal::World&>(world), *renderer.get());
}

void Game::processInput() {
  gameController->processInput();
}

void Game::renderHook() {
  gameRenderer->render((currentTime-lastRender)/1000., *camera.get());
  uiRenderer->render();
}

} /* namespace glPortal */
