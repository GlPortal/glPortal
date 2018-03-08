#ifndef GLPORTAL_GAME_HPP
#define GLPORTAL_GAME_HPP

#include "World.hpp"
#include "GameController.hpp"

#include <radix/Window.hpp>
#include <radix/renderer/Renderer.hpp>
#include <radix/env/Config.hpp>
#include <radix/BaseGame.hpp>
#include <radix/physics/PhysicsDebugDraw.hpp>
#include <radix/renderer/PortalRenderer.hpp>

#include "glPortal/renderer/GameRenderer.hpp"
#include "glPortal/renderer/UiRenderer.hpp"
#include <glPortal/GameController.hpp>
#include <glPortal/input/InputManager.hpp>

namespace glPortal {

class Game : public radix::BaseGame {
public:
  Game();

  void postSetup() override {}

  void onPreStartWorld() override;
  void onPostStartWorld() override;
  void onPreStopWorld() override;
  void onPostStopWorld() override;

  void processInput() override;
  void update() override;

  GameController* getGameController() {
    return gameController.get();
  }

private:
  std::unique_ptr<GameController> gameController;
  std::unique_ptr<radix::PhysicsDebugDraw> physicsDebugDraw;
  std::unique_ptr<GameRenderer> gameRenderer;
  std::unique_ptr<UiRenderer> uiRenderer;
  std::unique_ptr<radix::PortalRenderer> portalRenderer;

  double dtime;
  
  void initHook() override;
  void removeHook();
  void customTriggerHook() override;
  void initRenderers();
  void addRenderers();

};

} /* namespace glPortal */

#endif /* GLPORTAL_GAME_HPP */
