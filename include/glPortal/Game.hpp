#ifndef GAME_HPP
#define GAME_HPP

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

namespace glPortal {

class Game : public radix::BaseGame {
public:
  Game();

  void onPreStartWorld() override;
  void onPostStartWorld() override;
  void onPreStopWorld() override;
  void onPostStopWorld() override;

  void processInput() override;
  void update() override;

  GameController& getGameController() { return *gameController; }
private:
  bool showSplash;
  std::unique_ptr<radix::PhysicsDebugDraw> m_physDebugDraw;
  std::unique_ptr<GameController> gameController;
  std::unique_ptr<GameRenderer> gameRenderer;
  std::unique_ptr<UiRenderer> uiRenderer;
  std::unique_ptr<radix::PortalRenderer> portalRenderer;
  void initHook() override;
  void removeHook();
  void customTriggerHook() override;
  void initFunctionStack();
  void initRenderers();
  void addRenderers();
  double dtime;
};

} /* namespace glPortal */

#endif /* GAME_HPP */
