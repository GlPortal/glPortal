#include "Input.hpp"

#include <SDL2/SDL_keyboard.h>

namespace glPortal {

// Uses C++11's vector<bool> specialization, space-efficient (1 bool = 1 bit)
std::vector<bool> Input::keystates(SDL_NUM_SCANCODES);

void Input::keyPressed(int key, int mod) {
  keystates[key] = true;
}

void Input::keyReleased(int key, int mod) {
  keystates[key] = false;
}

bool Input::isKeyDown(int key) {
  return keystates[key];
}

} /* namespace glPortal */
