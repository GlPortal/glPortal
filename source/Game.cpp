#include <glPortal/Game.hpp>


#include <radix/entities/Trigger.hpp>
#include <radix/entities/Player.hpp>
#include <radix/simulation/Physics.hpp>

#include <glPortal/trigger/PortalTeleport.hpp>

namespace glPortal {

Game::Game() {
  windowTitle = "GlPortal";
  defaultMap = "/maps/n1.xml";
  gameController = std::make_unique<GameController>(this);
  inputManager = std::make_unique<InputManager>(this);
}

void Game::onPreStartWorld() {
  gameController->init();
  initRenderers();
  addRenderers();
  world->simulations.findFirstOfType<radix::simulation::Physics>().setDebugDraw(physicsDebugDraw.get());
  world->entityPairs.insert(std::make_pair("portalPairs", std::vector<radix::EntityPair>()));
}
void Game::onPostStartWorld() {}
void Game::onPreStopWorld() {}
void Game::onPostStopWorld() {
  gameRenderer.reset();
  uiRenderer.reset();
}

void Game::onInit() {
  physicsDebugDraw.reset(new radix::PhysicsDebugDraw);
}

void Game::onShutdown() {
  physicsDebugDraw.reset(nullptr);
}

void Game::customTriggerHook() {
  customTriggers.push_back(PortalTeleport());
}

void Game::processInput() {
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
