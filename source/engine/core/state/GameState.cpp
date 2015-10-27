#include "GameState.hpp"
#include <engine/env/Environment.hpp>

namespace glPortal {

GameState::GameState() {
  stateFunctionStack.push(&GameState::handleInputSplash);
}
void GameState::handleInput(Game& game, const SDL_Event &event){
  HandleEventGameFunction stateFunction = stateFunctionStack.top();
  stateFunction(game, event);
  stateFunctionStack.pop();
}

void GameState::handleInputRunning(Game& game, const SDL_Event &event){}
void GameState::handleInputPaused(Game& game, const SDL_Event &event){}
void GameState::handleInputSplash(Game& game, const SDL_Event &event){}
void GameState::handleInputMenu(Game& game, const SDL_Event &event){}
void GameState::handleInputGameOverScreen(Game& game, const SDL_Event &event){}
void GameState::handleInputWinScreen(Game& game, const SDL_Event &event){}
}
