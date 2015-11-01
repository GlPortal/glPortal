#include "GameState.hpp"
#include "../../../Input.hpp"
#include <engine/env/Environment.hpp>

namespace glPortal {
  
void GameState::handleInput(Game& game){
  HandleGameFunction stateFunction = game.getWorld()->stateFunctionStack.top();
  stateFunction(game);
}

void GameState::handleRunning(Game& game){
}
void GameState::handlePaused(Game& game){}

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
