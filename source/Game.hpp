#ifndef GAME_HPP
#define GAME_HPP

#include "Window.hpp"
#include "World.hpp"
#include "util/sdl/Fps.hpp"
#include <SDL2/SDL_events.h>

const int UPDATE_RATE = 50;
const int SKIP_TIME = 1000 / UPDATE_RATE;
const int MAX_SKIP = 5;

namespace glPortal {

class Game {
public:
  // FIXME: make non-static
  static Fps fps;

  Game();
  void update();
  void close();
  void handleEvent(const SDL_Event &event);
private:
  Window window;
  World world;

  bool closed;
};

} /* namespace glPortal */

#endif /* GAME_HPP */
