#include "PlayerState.hpp"
#include <engine/env/Environment.hpp>

namespace glPortal {

PlayerState::PlayerState() {
  //  stateFunctionStack.push();
}
void PlayerState::handleInput(Entity& player, const SDL_Event &event){}
void PlayerState::handleInputRunning(Entity& player, const SDL_Event &event){}
void PlayerState::handleInputJumping(Entity& player, const SDL_Event &event){}
}
