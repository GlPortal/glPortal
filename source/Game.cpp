#include "Game.hpp"

#include <stdexcept>
#include <string>
#include <cstdio>
#include <iostream>

#include <SDL2/SDL_timer.h>

#include <radix/component/Transform.hpp>
#include <radix/component/Player.hpp>
#include <radix/env/Environment.hpp>
#include <radix/env/ArgumentsParser.hpp>
#include <radix/env/Util.hpp>
#include <radix/map/XmlMapLoader.hpp>
#include <radix/SoundManager.hpp>
#include <radix/core/diag/Throwables.hpp>
#include <radix/system/PlayerSystem.hpp>
#include <radix/system/PhysicsSystem.hpp>
#include "renderer/UiRenderer.hpp"
#include <SDL2/SDL_keyboard.h>

using namespace radix;

namespace glPortal {

Game::Game() : config(){
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
  renderer = std::make_unique<Renderer>(world);
  camera = std::make_unique<Camera>();
  { World::SystemTransaction st = world.systemTransact();
    st.addSystem<PlayerSystem>();
    st.addSystem<PhysicsSystem>();
  }
  nextUpdate = SDL_GetTicks(), lastUpdate = 0, lastRender = 0;

  renderer->setViewport(&window);

  gameController = std::make_unique<GameController>(this);
  gameRenderer = std::make_unique<GameRenderer>(static_cast<glPortal::World&>(world), *renderer.get());
  uiRenderer = std::make_unique<UiRenderer>(static_cast<glPortal::World&>(world), *renderer.get());
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

  fps.countCycle();
  window.swapBuffers();
  lastRender = currentTime;
}

} /* namespace glPortal */
