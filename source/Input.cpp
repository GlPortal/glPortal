#include "Input.hpp"

namespace glPortal {

bool Input::keystates[] = { false };

void Input::keyPressed(int key, int mod) {
  //If the key lies outside the range of our buffer, discard it
  if (key < 0 || key >= KEY_BUFFER) {
    return;
  }
  keystates[key] = true;
}

void Input::keyReleased(int key, int mod) {
  //If the key lies outside the range of our buffer, discard it
  if (key < 0 || key >= KEY_BUFFER) {
    return;
  }
  keystates[key] = false;
}

bool Input::isKeyDown(int key) {
  return keystates[key];
}

} /* namespace glPortal */
