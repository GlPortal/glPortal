#include "Fps.hpp"

#include <SDL2/SDL_timer.h>
#include <string>
#include <cstdio>
#include <iostream>

namespace glPortal {

Fps::Fps() {
  fps = 0;
  lastFpsTime = SDL_GetTicks();
  rendered = 0;
}

void Fps::countCycle(){
  rendered++;
}

/**
  * Dump current FPS to stdout
  */
void Fps::dump(){
  std::cout << "FPS: " << getFps() << std::endl;
}
 
int Fps::getFps() {
    if (SDL_GetTicks() - lastFpsTime > 1000) {
      lastFpsTime = SDL_GetTicks();
      fps = rendered;
      rendered = 0;
    }
    return fps;
}

} /* namespace glPortal */
