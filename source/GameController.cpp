#include "GameController.hpp"
#include <SDL2/SDL_keyboard.h>
#include <stdexcept>
#include <string>
#include <cstdio>
#include <iostream>
#include <memory>

#include "engine/env/Environment.hpp"
#include "engine/env/ArgumentsParser.hpp"
#include <engine/env/System.hpp>
#include <engine/SoundManager.hpp>
#include <engine/core/event/Observer.hpp>
#include <engine/core/event/Dispatcher.hpp>
#include <engine/core/event/observer/MusicObserver.hpp>
#include <engine/GWENInput.hpp>
#include "Input.hpp"
#include "Game.hpp"

namespace glPortal {

GameController::GameController(Game *game) {
  this->game = game;
  this->world = game->getWorld();
  this->playerState = std::make_unique<PlayerState>();
  this->gameState   = std::make_unique<GameState>();
}

void GameController::handleInput() {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    game->getWindow().gwenInput->processEvent(event);
    handleEvent(event);
  }
}

void GameController::handleEvent(const SDL_Event &event) {
  if (event.type == SDL_QUIT) {
    game->close();
  }

  //  this->playerState->handleEvent(this->world->getPlayer(), event);
  this->gameState->handleInput(*this->game);

  int key = event.key.keysym.scancode;
  int mod = event.key.keysym.mod;
  int sym = event.key.keysym.sym;
  // int cursor = event.edit.start;
  // int selection_len = event.edit.length;

  switch (event.type) {
  case SDL_TEXTINPUT:
    Input::addToBuffer(event.text.text);
    break;
  case SDL_TEXTEDITING:

    break;
  case SDL_KEYDOWN:
    if (sym == SDLK_BACKSPACE) {
      Input::truncateCharBuffer();
    }

    if (sym == SDLK_RETURN) {
      Input::clearBuffer();
    }

    if (sym == SDLK_F1) {
      world->isPhysicsDebugEnabled = not world->isPhysicsDebugEnabled;
    } else if (sym == SDLK_F5) {
      if (Input::isKeyDown(SDL_SCANCODE_LSHIFT) || Input::isKeyDown(SDL_SCANCODE_RSHIFT)) {
        // Enable reload-on-change (inotify on Linux)
      }

      world->loadScene(world->currentScenePath);
    }
    
    Input::keyPressed(key, mod);
    if (key == SDL_SCANCODE_Q) {
      game->close();
    }
    break;
  case SDL_KEYUP:
    Input::keyReleased(key, mod);
    break;
  case SDL_MOUSEBUTTONDOWN:
    if (event.button.button == SDL_BUTTON_LEFT) {
      world->shootPortal(1);
    }
    if (event.button.button == SDL_BUTTON_RIGHT) {
      world->shootPortal(2);
    }
    if (event.button.button == SDL_BUTTON_MIDDLE and
        Environment::getConfig().isHidePortalsByClick()) {
      world->hidePortals();
    }
    break;
  }
}

} /* namespace glPortal */
