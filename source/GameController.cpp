#include "GameController.hpp"
#include <SDL2/SDL_keyboard.h>
#include <stdexcept>
#include <string>
#include <cstdio>
#include <iostream>

#include "engine/env/Environment.hpp"
#include "engine/env/ArgumentsParser.hpp"
#include <engine/env/System.hpp>
#include <engine/SoundManager.hpp>
#include <engine/core/event/Observer.hpp>
#include <engine/core/event/Dispatcher.hpp>
#include <engine/core/event/observer/MusicObserver.hpp>
#include "Input.hpp"
#include "Game.hpp"

namespace glPortal {

GameController::GameController(Game *game) {
  this->game = game;
  this->world = game->getWorld();
}

void GameController::handleInput() {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    handleEvent(event);
  }
}

void GameController::handleEvent(const SDL_Event &event) {
  if (event.type == SDL_QUIT) {
    game->close();
  }
  if (event.type == SDL_KEYDOWN) {
    int key = event.key.keysym.scancode;
    int mod = event.key.keysym.mod;

    Input::keyPressed(key, mod);

    if (key == SDL_SCANCODE_Q) {
      game->close();
    }
  }
  if (event.type == SDL_KEYUP) {
    int key = event.key.keysym.scancode;
    int mod = event.key.keysym.mod;

    Input::keyReleased(key, mod);
  }
  if (event.type == SDL_MOUSEBUTTONDOWN) {
    if (event.button.button == SDL_BUTTON_LEFT) {
      world->shootPortal(1);
    }
    if (event.button.button == SDL_BUTTON_RIGHT) {
      world->shootPortal(2);
    }
    if (event.button.button == SDL_BUTTON_MIDDLE  and Environment::getConfig().isHidePortalsByClick()) {
      world->hidePortals();
    }
  }
  
  // If F5 released, reload the scene
  if (wasF5Down and not Input::isKeyDown(SDL_SCANCODE_F5)) {
    if (Input::isKeyDown(SDL_SCANCODE_LSHIFT) || Input::isKeyDown(SDL_SCANCODE_RSHIFT)) {
      // Enable reload-on-change (inotify on Linux)
    }

    world->loadScene(world->currentScenePath);
  }
  wasF5Down = Input::isKeyDown(SDL_SCANCODE_F5);
  // If Tab released, toggle editor
  if (wasTabDown and not Input::isKeyDown(SDL_SCANCODE_TAB)) {
    world->isEditorShown = not world->isEditorShown;
  }
  wasTabDown = Input::isKeyDown(SDL_SCANCODE_F5);

}
}
