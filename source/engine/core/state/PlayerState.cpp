#include "PlayerState.hpp"
#include <engine/env/Environment.hpp>

namespace glPortal {

PlayerState::PlayerState() {
  stateFunctionStack.push(&PlayerState::handleInputStanding);
}
void PlayerState::handleInput(Entity& player, const SDL_Event &event){
  HandleEventFunction stateFunction = stateFunctionStack.top();
  stateFunction(player, event);
  stateFunctionStack.pop();
}
void PlayerState::handleInputStanding(Entity& player, const SDL_Event &event){}
void PlayerState::handleInputRunning(Entity& player, const SDL_Event &event){}
void PlayerState::handleInputJumping(Entity& player, const SDL_Event &event){}
}
