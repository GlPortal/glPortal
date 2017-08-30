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
}

void Game::initHook() {
  initFunctionStack();
  gameController = std::make_unique<GameController>(this);
  initRenderers();
  addRenderers();

  m_physDebugDraw.reset(new radix::PhysicsDebugDraw);
  world.simulations.findFirstOfType<radix::simulation::Physics>().setDebugDraw(m_physDebugDraw.get());
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
  dtime = (currentTime-lastRender)/1000.;
  BaseGame::update();
}

void Game::initRenderers() {
  World& worldReference = static_cast<glPortal::World&>(world);
  radix::Renderer& rendererReference = *renderer.get();
  gameRenderer =
    std::make_unique<GameRenderer>(worldReference, rendererReference, world.camera.get(), &dtime);
  uiRenderer =
    std::make_unique<UiRenderer>(worldReference, rendererReference);
 }

void Game::addRenderers() {
  renderer->addRenderer(*gameRenderer);
  renderer->addRenderer(*uiRenderer);
}

} /* namespace glPortal */
