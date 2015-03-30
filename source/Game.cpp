#include "Game.hpp"

#include <SDL2/SDL_keyboard.h>
#include <SDL2/SDL_timer.h>
#include <stdexcept>
#include <string>
#include <cstdio>
#include <iostream>

#include "engine/env/ConfigFileParser.hpp"
#include "engine/env/Environment.hpp"
#include <engine/env/System.hpp>
#include <engine/SoundManager.hpp>
#include <util/sdl/Fps.hpp>
#include "Input.hpp"
#include <SDL.h>
namespace glPortal {

Game::Game() : closed(false) {
  window.createFromConfig();
  
  try {
    SoundManager::Init();
    world.create();
    update();
  }
  catch (std::runtime_error &e) {
    System::log(std::string("Runtime Error: " + std::string(e.what())));
  }
}

void Game::update() {
  SDL_Event event;
  int skipped;
  unsigned int nextUpdate = SDL_GetTicks();
  Fps fps;
  
  while (not closed) {
    skipped = 0;
    fps.getFps();
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

void Game::handleEvent(SDL_Event event){
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
  }
}

} /* namespace glPortal */

using namespace glPortal;

Window window;

int main(int argc, char *argv[]) {
  Environment::init(argc, argv);
  Game game;

  return 0;
}
