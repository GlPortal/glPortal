#ifndef GAME_HPP
#define GAME_HPP

#include <radix/Window.hpp>
#include <radix/renderer/Renderer.hpp>
#include <radix/env/Config.hpp>
#include <radix/BaseGame.hpp>

#include "World.hpp"
#include "renderer/GameRenderer.hpp"
#include "renderer/UiRenderer.hpp"

namespace glPortal {

const int UPDATE_RATE = 60;
const int SKIP_TIME = 1000 / UPDATE_RATE;
const int MAX_SKIP = 5;

class Game : public radix::BaseGame {
public:
  Game();
  void render();
private:
  std::unique_ptr<GameRenderer> gameRenderer;
  std::unique_ptr<UiRenderer> uiRenderer;
  void loadMap();
  void init();
  radix::Config config;
};

} /* namespace glPortal */

#endif /* GAME_HPP */
