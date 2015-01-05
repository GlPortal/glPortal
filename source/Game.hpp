#ifndef GAME_HPP
#define GAME_HPP

#include "Window.hpp"
#include "World.hpp"
#include <SDL2/SDL_events.h>

const int UPDATE_RATE = 50;
const int SKIP_TIME = 1000 / UPDATE_RATE;
const int MAX_SKIP = 5;

namespace glPortal {

class ConfigFileParser;

class Game {
public:
  Game();
  void update();
  void close();
  void handleEvent(SDL_Event event);
private:
  Window window;
  World world;

  bool closed;
  ConfigFileParser * config;
};

} /* namespace glPortal */

#endif /* GAME_HPP */
