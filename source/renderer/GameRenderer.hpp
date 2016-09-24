#ifndef GAMERENDERER_HPP
#define GAMERENDERER_HPP

#include <memory>
#include <string>

#include <radix/core/math/Vector3f.hpp>

namespace radix {
  struct RenderContext;
  class Camera;
  class Renderer;
  class Entity;
}

namespace glPortal {

class World;

class GameRenderer {
public:
  GameRenderer(World& w, radix::Renderer& ren);

  void render(double dtime, const radix::Camera &cam);
  void initCamera();

  /**
   * Renders the scene with provided camera parameters
   */
  void renderScene(radix::RenderContext &rc);

  void renderViewFrames(radix::RenderContext &rc);
  void renderViewFrameStencil(radix::RenderContext &rc);

  /**
   * Renders all the entities in the scene regardless of shading
   */
  void renderEntities(radix::RenderContext &rc);

  /**
   * Renders a single entity regardless of shading
   */
  void renderEntity(radix::RenderContext &rc, const radix::Entity &e);

  /**
   * Renders the player character using ambient shading
   */
  void renderPlayer(radix::RenderContext &rc);

private:
  World &world;
  radix::Renderer &renderer;
  std::unique_ptr<radix::Camera> camera;
  std::unique_ptr<radix::RenderContext> rc;
  int viewportWidth, viewportHeight;

  double time;
};

} /* namespace glPortal */

#endif /* GAMERENDERER_HPP */
