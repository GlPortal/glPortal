#ifndef GAME_HPP
#define GAME_HPP

#include <radix/Window.hpp>
#include <radix/renderer/Renderer.hpp>
#include <radix/env/Config.hpp>
#include <radix/BaseGame.hpp>
#include <radix/screen/Screen.hpp>
#include <radix/renderer/ScreenRenderer.hpp>
#include "World.hpp"
#include "renderer/GameRenderer.hpp"
#include "renderer/UiRenderer.hpp"
#include "GameController.hpp"

namespace glPortal {

class Game : public radix::BaseGame {
public:
  Game();
  void processInput();
private:
  std::shared_ptr<radix::Screen> screen;
  std::unique_ptr<radix::ScreenRenderer> screenRenderer;
  std::unique_ptr<GameController> gameController;
  std::unique_ptr<GameRenderer> gameRenderer;
  std::unique_ptr<UiRenderer> uiRenderer;
  void renderHook();
  void init();
};

} /* namespace glPortal */

#endif /* GAME_HPP */
