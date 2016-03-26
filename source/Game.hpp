#ifndef GAME_HPP
#define GAME_HPP

#include <radix/Window.hpp>

#include "World.hpp"
#include "util/sdl/Fps.hpp"

namespace glPortal {

const int UPDATE_RATE = 50;
const int SKIP_TIME = 1000 / UPDATE_RATE;
const int MAX_SKIP = 5;

class GameController;
class Game {
public:
  // FIXME: make non-static
  static Fps fps;

  Game();
  Game(Game&) = delete;
  Game(Game&&) = delete;

  void update();
  void close();
  World* getWorld();
  inline radix::Window& getWindow() {
    return window;
  }

private:
  radix::Window window;
  World world;
  bool closed;
};

} /* namespace glPortal */

#endif /* GAME_HPP */
