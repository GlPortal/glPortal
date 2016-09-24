#ifndef GAME_HPP
#define GAME_HPP

#include <radix/Window.hpp>
#include <radix/renderer/Renderer.hpp>
#include <radix/env/Config.hpp>

#include "World.hpp"
#include "util/sdl/Fps.hpp"
#include "renderer/GameRenderer.hpp"
#include "renderer/UiRenderer.hpp"

namespace glPortal {

const int UPDATE_RATE = 60;
const int SKIP_TIME = 1000 / UPDATE_RATE;
const int MAX_SKIP = 5;

class GameController;
class Game {
public:
  static Fps fps;

  Game();
  Game(Game&) = delete;
  Game(Game&&) = delete;
  bool isRunning();
  void processInput();
  void update();
  void cleanUp();
  void close();
  void render();
  World* getWorld();
  inline radix::Window& getWindow() {
    return window;
  }

private:
  unsigned int currentTime = 0, nextUpdate = 0, lastUpdate = 0, lastRender = 0;
  radix::Window window;
  World world;
  std::unique_ptr<GameRenderer> gameRenderer;
  std::unique_ptr<UiRenderer> uiRenderer;
  std::unique_ptr<radix::Renderer> renderer;
  std::unique_ptr<radix::Camera> camera;
  bool closed;
  void prepareCamera();
  void init();
  void loadMap();
  radix::Config config;
};

} /* namespace glPortal */

#endif /* GAME_HPP */
