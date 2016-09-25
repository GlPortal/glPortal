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

Game::Game() :
  world(window),
  closed(false),
  config(Environment::getConfig()){
  window.create("GlPortal");

  try {
    SoundManager::init();
    init();
    loadMap();
  } catch (std::runtime_error &e) {
    Util::Log(Error) << "Runtime Error: " << e.what();
  }
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

  renderer->render((currentTime-lastRender)/1000., *camera.get());
  radix::RenderContext renderContext(*renderer.get());
  UiRenderer::render(renderContext, (World)world);

  fps.countCycle();
  window.swapBuffers();
  lastRender = currentTime;
}

} /* namespace glPortal */
