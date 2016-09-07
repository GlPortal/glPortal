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
#include <util/sdl/Fps.hpp>
#include "renderer/UiRenderer.hpp"
#include <SDL2/SDL_keyboard.h>

using namespace radix;

namespace glPortal {

Fps Game::fps;

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

void Game::init() {
  radix::Environment::init();
  radix::ArgumentsParser::populateConfig();

  if(config.cursorVisibility) {
    window.unlockMouse();
  } else {
    window.lockMouse();
  }
  world.create();
  renderer = std::make_unique<Renderer>(world);
  camera = std::make_unique<Camera>();
  { World::SystemTransaction st = world.systemTransact();
    st.addSystem<PlayerSystem>();
    st.addSystem<PhysicsSystem>();
  }
  nextUpdate = SDL_GetTicks(), lastUpdate = 0, lastRender = 0;

  renderer->setViewport(&window);
}

bool Game::isRunning() {
  return !closed;
}

World* Game::getWorld() {
  return &world;
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

void Game::update() {
  int skipped = 0;
  currentTime = SDL_GetTicks();
  //Update the game if it is time
  while (currentTime > nextUpdate && skipped < MAX_SKIP) {
    SoundManager::update(world.getPlayer());
    world.update(TimeDelta::msec(currentTime-lastUpdate));
    lastUpdate = currentTime;
    nextUpdate += SKIP_TIME;
    skipped++;
  }
}

void Game::processInput() {
  window.processEvents();
  if (window.isKeyDown(SDL_SCANCODE_Q)) {
    close();
  }
}

void Game::cleanUp() {
  world.destroy();
  window.close();
}

void Game::render() {
  camera->setPerspective();
  int viewportWidth, viewportHeight;
  window.getSize(&viewportWidth, &viewportHeight);
  camera->setAspect((float)viewportWidth / viewportHeight);
  const Transform &playerTransform = world.getPlayer().getComponent<Transform>();
  Vector3f headOffset(0, playerTransform.getScale().y, 0);
  camera->setPosition(playerTransform.getPosition() + headOffset);
  const Player &playerComponent = world.getPlayer().getComponent<Player>();
  camera->setOrientation(playerComponent.getHeadOrientation());

  renderer->render((currentTime-lastRender)/1000., *camera.get());
  radix::RenderContext renderContext(*renderer.get());
  UiRenderer::render(renderContext, world);

  fps.countCycle();
  window.swapBuffers();
  lastRender = currentTime;
}


void Game::close() {
  closed = true;
}

} /* namespace glPortal */
