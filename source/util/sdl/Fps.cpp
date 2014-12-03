#include "Fps.hpp"

#include <SDL2/SDL_timer.h>
#include <string>
#include <cstdio>
#include <iostream>

namespace glPortal {

Fps::Fps() {
  fps = 0;
  lastFpsTime = SDL_GetTicks();
  skipped = 0;
  rendered = 0;
}

void Fps::countCycle() {
  rendered++;
}

/**
  * Dump current FPS to stdout
  */
void Fps::dump() {

}

int Fps::getFps() {
    if (SDL_GetTicks() - lastFpsTime > 1000) {
      lastFpsTime = SDL_GetTicks();
      fps = rendered;
      rendered = 0;
      std::cout << "FPS: " << fps << std::endl;
    }
    return fps;
}

} /* namespace glPortal */
