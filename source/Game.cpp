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
#include <radix/renderer/Renderer.hpp>
#include <radix/system/PlayerSystem.hpp>
#include <radix/system/PhysicsSystem.hpp>
#include <util/sdl/Fps.hpp>
#include "renderer/UiRenderer.hpp"

using namespace radix;

namespace glPortal {

Fps Game::fps;

Game::Game() :
  world(window),
  closed(false) {
  window.create("GlPortal");

  try {
    SoundManager::init();
    world.create();
    { World::SystemTransaction st = world.systemTransact();
      st.addSystem<PlayerSystem>();
      st.addSystem<PhysicsSystem>();
    }
    XmlMapLoader ldr(world);
    ldr.load(Environment::getDataDir() + "/maps/n1.xml");
    update();
  } catch (std::runtime_error &e) {
    Util::Log(Error) << "Runtime Error: " << e.what();
  }
}

World* Game::getWorld() {
  return &world;
}

void Game::update() {
  unsigned int nextUpdate = SDL_GetTicks(), lastUpdate = 0, lastRender = 0;
  Renderer renderer(world);
  renderer.setViewport(&window);
  Camera camera;

  while (not closed) {
    window.processEvents();

    int skipped = 0;
    unsigned int currentTime = SDL_GetTicks();
    //Update the game if it is time
    while (currentTime > nextUpdate && skipped < MAX_SKIP) {
      SoundManager::update(world.getPlayer());
      world.update((currentTime-lastUpdate)/1000.);
      lastUpdate = currentTime;
      nextUpdate += SKIP_TIME;
      skipped++;
    }

    camera.setPerspective();
    int vpWidth, vpHeight;
    window.getSize(&vpWidth, &vpHeight);
    camera.setAspect((float)vpWidth / vpHeight);
    const Transform &plrTform = world.getPlayer().getComponent<Transform>();
    camera.setPosition(plrTform.getPosition() + Vector3f(0, plrTform.getScale().y/2, 0));
    const Player &plrComp = world.getPlayer().getComponent<Player>();
    camera.setOrientation(plrComp.getHeadOrientation());

    renderer.render((currentTime-lastRender)/1000., camera);
    radix::RenderContext rc(renderer);
    UiRenderer::render(rc, world);
    lastRender = currentTime;
    fps.countCycle();
    window.swapBuffers();
  }
  world.destroy();
  window.close();
}

void Game::close() {
  closed = true;
}

} /* namespace glPortal */

using namespace glPortal;

int main(const int argc, char *argv[]) {
  Util::Init();
  ArgumentsParser::setEnvironmentFromArgs(argc, argv);
  Environment::init();
  ArgumentsParser::populateConfig();

  try {
    Game game;
  } catch (Exception::Error &err) {
    Util::Log(Error, err.source()) << err.what();
  }

  return 0;
}
