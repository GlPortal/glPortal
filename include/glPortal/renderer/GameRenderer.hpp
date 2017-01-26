#ifndef GAME_RENDERER_HPP
#define GAME_RENDERER_HPP

#include <memory>
#include <string>

#include <radix/core/math/Vector3f.hpp>
#include <radix/renderer/SubRenderer.hpp>

namespace radix {
  struct RenderContext;
  class Camera;
  class Renderer;
  class Entity;
}

namespace glPortal {

class World;

class GameRenderer : public radix::SubRenderer{
public:
  GameRenderer(World& w, radix::Renderer& ren, radix::Camera* cam, double* ptime);

  void render();

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
  radix::Camera* camera;
  double* dtime;
  double time;
};

} /* namespace glPortal */

#endif /* GAME_RENDERER_HPP */
