#include "GameState.hpp"
#include "../../../Input.hpp"
#include <engine/env/Environment.hpp>

namespace glPortal {
const int GameState::POP_STACK  = 0;
const int GameState::DO_NOTHING = 1;
  
GameState::GameState() {
  stateFunctionStack.push(&GameState::handleRunning);
  stateFunctionStack.push(&GameState::handleSplash);
}
void GameState::handleInput(Game& game){
  HandleGameFunction stateFunction = stateFunctionStack.top();
  if (stateFunction(game) == GameState::POP_STACK){
    stateFunctionStack.pop();
  }
  
}

int GameState::handleRunning(Game& game){
  return GameState::DO_NOTHING;;
}
void GameState::handlePaused(Game& game){}

int GameState::handleSplash(Game& game){
  if (Input::isKeyDown(SDL_SCANCODE_RETURN)){
    game.getWorld()->scene->screen->enabled = false;
    return GameState::POP_STACK;
  }
}
void GameState::handleMenu(Game& game){}
void GameState::handleGameOverScreen(Game& game){}
void GameState::handleWinScreen(Game& game){}
}
