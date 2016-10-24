#include "Game.hpp"

#include <iostream>

#include <radix/map/XmlMapLoader.hpp>
#include <radix/SoundManager.hpp>
#include <radix/screen/XmlScreenLoader.hpp>

#include "GameState.hpp"

using namespace radix;

namespace glPortal {

Game::Game() {
  windowTitle = "GlPortal";
  defaultMap = "/maps/n1.xml";
  BaseGame();
  try {
    SoundManager::init();
    init();
    loadMap();
    renderer->init();
  } catch (std::runtime_error &e) {
    Util::Log(Error) << "Runtime Error: " << e.what();
  }
}

void Game::init() {
  BaseGame::init();
  world.stateFunctionStack.push(&GameState::handleRunning);
  world.stateFunctionStack.push(&GameState::handleSplash);
  gameController = std::make_unique<GameController>(this);
  gameRenderer = std::make_unique<GameRenderer>(static_cast<glPortal::World&>(world), *renderer.get());
  uiRenderer = std::make_unique<UiRenderer>(static_cast<glPortal::World&>(world), *renderer.get());
  screenRenderer = std::make_unique<radix::ScreenRenderer>(world, *renderer.get());
}

void Game::processInput() {
  gameController->processInput();
}

void Game::renderHook() {
  gameRenderer->render((currentTime-lastRender)/1000., *camera.get());
  uiRenderer->render();

  for (radix::Screen* screen : *gameWorld.getScreens()) {
    screenRenderer->renderScreen(*screen);
  }
  gameWorld.getScreens()->clear();
}

} /* namespace glPortal */
