#ifndef GAME_HPP
#define GAME_HPP

#include "Window.hpp"
#include "World.hpp"
#include "Input.hpp"
#include "engine/environment/Environment.hpp"

const int UPDATE_RATE = 25;
const int SKIP_TIME = 1000 / UPDATE_RATE;
const int MAX_SKIP = 5;

namespace glPortal {

class Game {
public:
  Game();
  void update();
  void close();
private:
  Window window;
  World world;
  bool closed;
  ConfigFileParser * config;
};

} /* namespace glPortal */

#endif /* GAME_HPP */
