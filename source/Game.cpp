#include <glPortal/Game.hpp>
#include <glPortal/GameState.hpp>

#include <iostream>

#include <radix/entities/Trigger.hpp>
#include <radix/entities/Player.hpp>
#include <radix/simulation/Physics.hpp>

#include <glPortal/trigger/PortalTeleport.hpp>

using namespace radix;

namespace glPortal {

Game::Game() {
  windowTitle = "GlPortal";
  defaultMap = "/maps/n1.xml";
  showSplash = true;
}

void Game::onPreStartWorld() {
  initFunctionStack();
  gameController = std::make_unique<GameController>(this);
  initRenderers();
  addRenderers();
  world->simulations.findFirstOfType<radix::simulation::Physics>().setDebugDraw(m_physDebugDraw.get());
  world->entityPairs.insert(std::make_pair("portalPairs", std::vector<EntityPair>()));
}
void Game::onPostStartWorld() {}
void Game::onPreStopWorld() {}
void Game::onPostStopWorld() {
  gameController.reset();
  gameRenderer.reset();
  uiRenderer.reset();
}

void Game::initHook() {
  m_physDebugDraw.reset(new radix::PhysicsDebugDraw);
}

void Game::customTriggerHook() {
  customTriggers.push_back(PortalTeleport());
}

void Game::initFunctionStack() {
  world->stateFunctionStack.push(&GameState::handleRunning);
  if (showSplash) {
    world->stateFunctionStack.push(&GameState::handleSplash);
    showSplash = false;
  }
}

void Game::processInput() {
  gameController->processInput();
}

void Game::update() {
  dtime = (currentTime-lastRender)/1000.;
  BaseGame::update();
}

void Game::initRenderers() {
  World& worldReference = static_cast<glPortal::World&>(*world);
  radix::Renderer& rendererReference = *renderer.get();
  gameRenderer =
    std::make_unique<GameRenderer>(worldReference, rendererReference, world->camera.get(), &dtime);
  uiRenderer =
    std::make_unique<UiRenderer>(worldReference, rendererReference);
  portalRenderer =
    std::make_unique<radix::PortalRenderer>(worldReference, rendererReference,
        this->gameWorld);
 }

void Game::addRenderers() {
  renderer->addRenderer(*gameRenderer);
  renderer->addRenderer(*uiRenderer);
  renderer->addRenderer(*portalRenderer);
}

} /* namespace glPortal */
