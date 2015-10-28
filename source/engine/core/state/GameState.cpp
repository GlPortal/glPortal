#include "GameState.hpp"
#include <engine/env/Environment.hpp>

namespace glPortal {

GameState::GameState() {
  stateFunctionStack.push(&GameState::handleSplash);
}
void GameState::handleInput(Game &game){
  HandleGameFunction stateFunction = stateFunctionStack.top();
  stateFunction(game);
  //stateFunctionStack.pop();
}

void GameState::handleRunning(Game &game){}
void GameState::handlePaused(Game &game){}
void GameState::handleSplash(Game &game){}
void GameState::handleMenu(Game &game){}
void GameState::handleGameOverScreen(Game &game){}
void GameState::handleWinScreen(Game &game){}
}
