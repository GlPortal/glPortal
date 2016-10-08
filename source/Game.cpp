#include "Game.hpp"

#include <iostream>

#include <radix/component/Player.hpp>
#include <radix/env/Environment.hpp>
#include <radix/env/ArgumentsParser.hpp>
#include <radix/map/XmlMapLoader.hpp>
#include <radix/SoundManager.hpp>
#include <radix/system/PlayerSystem.hpp>
#include <radix/system/PhysicsSystem.hpp>
#include <radix/screen/XMLScreenLoader.hpp>

#include "GameState.hpp"

using namespace radix;

namespace glPortal {

Game::Game() : config(){
  BaseGame();
  config = radix::Environment::getConfig();
  radix::ArgumentsParser::populateConfig(config);
  window.setConfig(config);
  window.create("GlPortal");

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
  if(config.cursorVisibility) {
    window.unlockMouse();
  } else {
    window.lockMouse();
  }
  world.setConfig(config);
  world.create();
  world.stateFunctionStack.push(&GameState::handleRunning); /* default gamestate */
  renderer = std::make_unique<Renderer>(world);
  camera = std::make_unique<Camera>();
  { World::SystemTransaction systemTransaction = world.systemTransact();
    systemTransaction.addSystem<PlayerSystem>();
    systemTransaction.addSystem<PhysicsSystem>();
  }
  nextUpdate = SDL_GetTicks(), lastUpdate = 0, lastRender = 0;

  renderer->setViewport(&window);

  screen = radix::XMLScreenLoader::loadScreen(Environment::getDataDir() + "/screens/test.xml");

  gameController = std::make_unique<GameController>(this);
  gameRenderer = std::make_unique<GameRenderer>(static_cast<glPortal::World&>(world), *renderer.get());
  uiRenderer = std::make_unique<UiRenderer>(static_cast<glPortal::World&>(world), *renderer.get());
  screenRenderer = std::make_unique<ScreenRenderer>(world, *renderer.get());
  screenshotCallbackHolder =
    world.event.addObserver(InputSource::KeyReleasedEvent::Type, [this](const radix::Event &event) {
        const int key =  ((InputSource::KeyReleasedEvent &) event).key;
        if (key == SDL_SCANCODE_G) {
          this->window.printScreenToFile(radix::Environment::getDataDir() + "/screenshot.bmp");
        }
      });

}

void Game::processInput() {
  gameController->processInput();
}

void Game::loadMap() {
  XmlMapLoader mapLoader(world);
  std::string mapPath = config.mapPath;
  if (mapPath.length() > 0) {
    mapLoader.load(mapPath);
  } else {
    mapLoader.load(Environment::getDataDir() + "/maps/n1.xml");
  }
}

void Game::render() {
  prepareCamera();

  gameRenderer->render((currentTime-lastRender)/1000., *camera.get());
  uiRenderer->render();
  screenRenderer->renderScreen(screen);

  fps.countCycle();
  window.swapBuffers();
  lastRender = currentTime;
}

} /* namespace glPortal */
