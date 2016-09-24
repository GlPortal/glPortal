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

  /**
   * Set the camera in the portal so rendering from that portal is possible
   * @param cam         The camera from which we look at the portal
   * @param dest        The camera to move inside the portal and point in the right direction
   * @param portal      The portal in which to place the camera
   * @param otherPortal The counterpart of the portal
   */
  static void setCameraInPortal(const radix::Camera &cam, radix::Camera &dest,
                const radix::Entity &portal, const radix::Entity &otherPortal);

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
