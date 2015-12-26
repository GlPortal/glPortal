#include "GameState.hpp"
#include "../../../Input.hpp"
#include <engine/env/Environment.hpp>
#include <engine/core/math/Vector4f.hpp>
#include <engine/component/Player.hpp>

namespace glPortal {

void GameState::handleInput(Game& game){
  HandleGameFunction stateFunction = game.getWorld()->stateFunctionStack.top();
  stateFunction(game);
}

void GameState::handleRunning(Game& game){
  if (Input::isKeyDown(SDL_SCANCODE_ESCAPE)){
    game.getWorld()->scene->player->getComponent<Player>().frozen = true;
    game.getWorld()->scene->screen->enabled = true;
    game.getWorld()->scene->screen->title = "Pause";
    game.getWorld()->scene->screen->text  = "Press escape to continue playing.";
    game.getWorld()->stateFunctionStack.push(&GameState::handlePaused);
  }

  if (Input::isKeyDown(SDL_SCANCODE_F2)){
    game.getWorld()->scene->player->getComponent<Player>().frozen = true;
    game.getWorld()->scene->terminal->enabled = true;
    game.getWorld()->stateFunctionStack.push(&GameState::handleTerminal);
    Input::clear();
    SDL_StartTextInput();
  }
}

void GameState::handlePaused(Game& game){
  if (Input::isKeyDown(SDL_SCANCODE_ESCAPE)){
    game.getWorld()->scene->screen->enabled = false;
    game.getWorld()->stateFunctionStack.pop();
    Input::clear();
    game.getWorld()->scene->player->getComponent<Player>().frozen = false;
  }
}

void GameState::handleSplash(Game& game){
  if (Input::isKeyDown(SDL_SCANCODE_RETURN)){
    game.getWorld()->scene->screen->enabled = false;
    game.getWorld()->stateFunctionStack.pop();
    Input::clear();
    game.getWorld()->scene->player->getComponent<Player>().frozen = false;
    game.getWorld()->scene->screen->enabled = false;
  }
}
void GameState::handleMenu(Game& game){}
void GameState::handleTerminal(Game& game){
  if (Input::isKeyDown(SDL_SCANCODE_F2)){
    game.getWorld()->scene->player->getComponent<Player>().frozen = false;
    game.getWorld()->scene->terminal->enabled = false;
    SDL_StopTextInput();
    game.getWorld()->stateFunctionStack.pop();
  }
}
void GameState::handleGameOverScreen(Game& game){}
void GameState::handleWinScreen(Game& game){}
}
