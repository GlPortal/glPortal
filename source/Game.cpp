#include "Game.hpp"

#include <SDL2/SDL_timer.h>
#include <stdexcept>
#include <string>
#include <cstdio>
#include <iostream>

#include "engine/env/ConfigFileParser.hpp"
#include "engine/env/Environment.hpp"
#include "engine/env/ArgumentsParser.hpp"
#include <engine/env/System.hpp>
#include <engine/SoundManager.hpp>
#include <engine/core/event/Observer.hpp>
#include <engine/core/event/Dispatcher.hpp>
#include <engine/core/event/observer/MusicObserver.hpp>
#include <util/sdl/Fps.hpp>
#include "GameController.hpp"

namespace glPortal {

Fps Game::fps;

Game::Game() : closed(false) {
  controller = new GameController(this);
  MusicObserver musicObserver;
  musicObserver.addCallback(Event::loadScene, std::bind(&MusicObserver::loadMap, musicObserver));
  window.create("GlPortal");

  try {
    SoundManager::Init();
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
  unsigned int nextUpdate = SDL_GetTicks();

  while (not closed) {
    int skipped = 0;
    //Update the game if it is time
    while (SDL_GetTicks() > nextUpdate && skipped < MAX_SKIP) {
      controller->handleInput();

      SoundManager::Update(world.getPlayer());
      world.update();
      nextUpdate += SKIP_TIME;
      skipped++;
    }
    world.render();
    fps.countCycle();
    window.swapBuffers();
  }
  world.destroy();
  window.close();
  delete (controller);
}

void Game::close() {
  closed = true;
}
} /* namespace glPortal */

using namespace glPortal;

Window window;

int main(int argc, char *argv[]) {
  System::Init();
  ArgumentsParser::setEnvironmentFromArgs(argc, argv);
  Environment::init();
  ArgumentsParser::populateConfig();
  Config::load();

  Game game;

  return 0;
}
