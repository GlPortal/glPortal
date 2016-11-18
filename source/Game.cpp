#include <glPortal/Game.hpp>
#include <glPortal/GameState.hpp>

#include <iostream>
#include <radix/component/Trigger.hpp>
#include <radix/component/Player.hpp>

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
  World& worldReference = static_cast<glPortal::World&>(world);
  radix::Renderer& rendererReference = *renderer.get();
  gameRenderer =
    std::make_unique<GameRenderer>(worldReference, rendererReference, camera.get(), &dtime);
  uiRenderer =
    std::make_unique<UiRenderer>(worldReference, rendererReference);

  renderer->addRenderer(*gameRenderer);
  renderer->addRenderer(*uiRenderer);

  Entity& entity = world.entities.create();
  Transform& transform = entity.addComponent<Transform>();
  transform.setPosition(world.getPlayer().getComponent<Transform>().getPosition());
  transform.setOrientation(world.getPlayer().getComponent<Transform>().getOrientation());
  transform.setScale(world.getPlayer().getComponent<Transform>().getScale());
  entity.addComponent<Trigger>([] (BaseGame* game) {}, [] (BaseGame* game) {},
                               [] (BaseGame* game) {
                                 Util::Log() << "wooh I moved!";
                               }, [] (BaseGame* game) {}
                               );
}

void Game::processInput() {
  gameController->processInput();
}

void Game::update() {
  BaseGame::update();
  dtime = (currentTime-lastRender)/1000.;
}

} /* namespace glPortal */
