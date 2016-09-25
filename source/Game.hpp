#ifndef GAME_HPP
#define GAME_HPP

#include <radix/Window.hpp>
#include <radix/renderer/Renderer.hpp>
#include <radix/env/Config.hpp>
#include <radix/BaseGame.hpp>

#include "World.hpp"

namespace glPortal {

const int UPDATE_RATE = 60;
const int SKIP_TIME = 1000 / UPDATE_RATE;
const int MAX_SKIP = 5;

class Game : public radix::BaseGame {
public:
  Game();
  void render();
private:
  void loadMap();
};

} /* namespace glPortal */

#endif /* GAME_HPP */
