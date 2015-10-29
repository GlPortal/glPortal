#include "GameState.hpp"
#include <engine/env/Environment.hpp>

namespace glPortal {

GameState::GameState() {
  stateFunctionStack.push(&GameState::handleRunning);
  stateFunctionStack.push(&GameState::handleSplash);
  stateFunctionStack.push(&GameState::handleMenu);
}
void GameState::handleInput(Game& game){
  HandleGameFunction stateFunction = stateFunctionStack.top();
  stateFunction(game);
}

void GameState::handleRunning(Game& game){}
void GameState::handlePaused(Game& game){}
void GameState::handleSplash(Game& game){
  
  //  stateFunctionStack.pop();
}
void GameState::handleMenu(Game& game){}
void GameState::handleGameOverScreen(Game& game){}
void GameState::handleWinScreen(Game& game){}
}
