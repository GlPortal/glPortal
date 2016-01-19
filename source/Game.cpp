#include "Game.hpp"

#include <stdexcept>
#include <string>
#include <cstdio>
#include <iostream>

#include <SDL2/SDL_timer.h>

#include <engine/env/Environment.hpp>
#include <engine/env/ArgumentsParser.hpp>
#include <engine/env/System.hpp>
#include <engine/SoundManager.hpp>
#include <engine/core/diag/Throwables.hpp>
#include <engine/core/event/Observer.hpp>
#include <engine/core/event/Dispatcher.hpp>
#include <engine/core/event/observer/MusicObserver.hpp>
#include <util/sdl/Fps.hpp>
#include "Input.hpp"
#include "GameController.hpp"

namespace glPortal {

Fps Game::fps;

Game::Game() : closed(false) {
  controller = std::make_unique<GameController>(this);
  MusicObserver musicObserver;
  musicObserver.addCallback(Event::loadScene, std::bind(&MusicObserver::loadMap, musicObserver));
  window.create("GlPortal");
  
  try {
    SoundManager::init();
    world.create();
    world.setRendererWindow(&window);
    update();
  }
  catch (std::runtime_error &e) {
    System::Log(Error) << "Runtime Error: " << e.what();
  }
}

World* Game::getWorld() {
  return &world;
}

void Game::update() {
  unsigned int nextUpdate = SDL_GetTicks(), lastUpdate = 0, lastRender = 0;

  while (not closed) {
    int skipped = 0;
    unsigned int currentTime = SDL_GetTicks();
    //Update the game if it is time
    while (currentTime > nextUpdate && skipped < MAX_SKIP) {
      controller->handleInput();

      SoundManager::update(world.getPlayer());
      world.update((currentTime-lastUpdate)/1000.);
      lastUpdate = currentTime;
      nextUpdate += SKIP_TIME;
      skipped++;
    }
    world.render((currentTime-lastRender)/1000.);
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
  System::Init();
  ArgumentsParser::setEnvironmentFromArgs(argc, argv);
  Environment::init();
  ArgumentsParser::populateConfig();

  try {
    Game game;
  } catch (Exception::Error &err) {
    System::Log(Error, err.source()) << err.what();
  }

  return 0;
}
