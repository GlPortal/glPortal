#include "Input.hpp"

#include <SDL2/SDL_keyboard.h>

namespace glPortal {

// Uses C++11's vector<bool> specialization, space-efficient (1 bool = 1 bit)
std::vector<bool> Input::keystates(SDL_NUM_SCANCODES);
std::string Input::charbuffer("");

void Input::keyPressed(int key, int mod) {
  keystates[key] = true;
}

void Input::keyReleased(int key, int mod) {
  keystates[key] = false;
}

bool Input::isKeyDown(int key) {
  return keystates[key];
}

void Input::addToBuffer(std::string character) {
  charbuffer.append(character);
}

std::string Input::getCharBuffer() {
  return charbuffer;
}

void Input::clearBuffer() {
  charbuffer = "";
}

void Input::clear() {
  keystates.clear();
}

} /* namespace glPortal */
