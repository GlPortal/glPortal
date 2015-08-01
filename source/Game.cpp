#include "Game.hpp"

#include <SDL2/SDL_keyboard.h>
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
#include "Input.hpp"

namespace glPortal {

Fps Game::fps;

Game::Game() : closed(false) {
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

void Game::update() {
  SDL_Event event;
  int skipped;
  unsigned int nextUpdate = SDL_GetTicks();

  while (not closed) {
    skipped = 0;
    //Update the game if it is time
    while (SDL_GetTicks() > nextUpdate && skipped < MAX_SKIP) {
      while (SDL_PollEvent(&event)) {
        handleEvent(event);
      }

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
}

void Game::close() {
  closed = true;
}

void Game::handleEvent(const SDL_Event &event) {
  if (event.type == SDL_QUIT) {
    closed = 1;
  }
  if (event.type == SDL_KEYDOWN) {
    int key = event.key.keysym.scancode;
    int mod = event.key.keysym.mod;

    Input::keyPressed(key, mod);

    if (key == SDL_SCANCODE_Q) {
      close();
    }
  }
  if (event.type == SDL_KEYUP) {
    int key = event.key.keysym.scancode;
    int mod = event.key.keysym.mod;

    Input::keyReleased(key, mod);
  }
  if (event.type == SDL_MOUSEBUTTONDOWN) {
    if (event.button.button == SDL_BUTTON_LEFT) {
      world.shootPortal(1);
    }
    if (event.button.button == SDL_BUTTON_RIGHT) {
      world.shootPortal(2);
    }
    if (event.button.button == SDL_BUTTON_MIDDLE  and Config::isHidePortalsByClick()) {
      world.hidePortals();
    }
  }
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
