#include "GameState.hpp"
#include "../../../Input.hpp"
#include <engine/env/Environment.hpp>
#include <engine/core/math/Vector4f.hpp>

namespace glPortal {

void GameState::handleInput(Game& game){
  HandleGameFunction stateFunction = game.getWorld()->stateFunctionStack.top();
  stateFunction(game);
}

void GameState::handleRunning(Game& game){
  game.getWorld()->scene->screen->enabled = false;
  if (Input::isKeyDown(SDL_SCANCODE_ESCAPE)){
    game.getWorld()->scene->screen->enabled = true;
    game.getWorld()->scene->screen->title = "Pause";
    game.getWorld()->scene->screen->text  = "Press escape to continue playing.";
    game.getWorld()->stateFunctionStack.push(&GameState::handlePaused);
  }
}

void GameState::handlePaused(Game& game){
  if (Input::isKeyDown(SDL_SCANCODE_ESCAPE)){
    game.getWorld()->scene->screen->enabled = false;
    game.getWorld()->stateFunctionStack.pop();
  }
}

void GameState::handleSplash(Game& game){
  if (Input::isKeyDown(SDL_SCANCODE_RETURN)){
    game.getWorld()->scene->screen->enabled = false;
    game.getWorld()->stateFunctionStack.pop();
  }
}
void GameState::handleMenu(Game& game){}
void GameState::handleGameOverScreen(Game& game){}
void GameState::handleWinScreen(Game& game){}
}
