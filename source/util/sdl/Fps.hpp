#ifndef FPS_HPP
#define FPS_HPP

#include <SDL2/SDL_events.h>

namespace glPortal {

class Fps {
public:
  Fps();
  int getFps();
  void countCycle();
  void dump();
private:
  unsigned int lastFpsTime;
  int fps;
  int skipped;
  int rendered;
};

} /* namespace glPortal */

#endif /* FPS_HPP */
