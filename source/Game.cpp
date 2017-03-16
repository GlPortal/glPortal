#include <glPortal/Game.hpp>
#include <glPortal/GameState.hpp>

#include <iostream>
#include <radix/component/Trigger.hpp>
#include <radix/component/Player.hpp>
#include <glPortal/trigger/PortalTeleport.hpp>

using namespace radix;

namespace glPortal {

Game::Game() {
  windowTitle = "GlPortal";
  defaultMap = "/packages/test/maps/teleportTriggerTerminalVelocity.xml";
}

void Game::initHook() {
  initFunctionStack();
  gameController = std::make_unique<GameController>(this);
  initRenderers();
  addRenderers();
}

void Game::customTriggerHook() {
  customTriggers.push_back(PortalTeleport());
}

void Game::initFunctionStack() {
  world.stateFunctionStack.push(&GameState::handleRunning);
  world.stateFunctionStack.push(&GameState::handleSplash);
}

void Game::processInput() {
  gameController->processInput();
}

void Game::update() {
  BaseGame::update();
  dtime = (currentTime-lastRender)/1000.;
}

void Game::initRenderers() {
  World& worldReference = static_cast<glPortal::World&>(world);
  radix::Renderer& rendererReference = *renderer.get();
  gameRenderer =
    std::make_unique<GameRenderer>(worldReference, rendererReference, camera.get(), &dtime);
  uiRenderer =
    std::make_unique<UiRenderer>(worldReference, rendererReference);
 }

void Game::addRenderers() {
  renderer->addRenderer(*gameRenderer);
  renderer->addRenderer(*uiRenderer);
}

} /* namespace glPortal */
