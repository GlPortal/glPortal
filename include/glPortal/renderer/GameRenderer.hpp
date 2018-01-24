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
  class FrameBuffer;
}

namespace glPortal {

class World;
class Portal;

class GameRenderer : public radix::SubRenderer {
public:
  /** 
  * @brief 
  * 
  * @param w
  * @param ren
  * @param cam
  * @param ptime
  */
  GameRenderer(World& w, radix::Renderer& ren, radix::Camera* cam, double* ptime);

  ~GameRenderer();

  /** 
  * @brief 
  */
  void render();

  /** 
  * @brief Renders the scene with provided camera parameters
  * 
  * @param rc
  */
  void renderScene(radix::RenderContext &renderContext);

  /** 
  * @brief 
  * 
  * @param renderContext
  */
  void renderDebugView(radix::RenderContext &renderContext);

  /** 
  * @brief 
  * 
  * @param rc
  */
  void renderViewFrames(radix::RenderContext &rc);

  /** 
  * @brief 
  * 
  * @param rc
  */
  void renderViewFrameStencil(radix::RenderContext &rc);

  /** 
  * @brief Renders all the entities in the scene regardless of shading
  * 
  * @param rc
  */
  void renderEntities(radix::RenderContext &rc);

  /**
   * @brief renderPortals render both portals color and blend
   * 
   * @param rc
   */
  void renderPortals(radix::RenderContext &rc);

void testPortalStencil(const Portal& portal, unsigned int occlusionQueryIdx,
                       radix::RenderContext& renderContext,
                       radix::Renderer& renderer,
                       const int stencilIndex);

  void renderSceneFromPortal(const radix::Camera& src,
      const Portal& portal1, const Portal& portal2,
      radix::RenderContext&, const int stencilIndex);

  /** 
  * @brief renderPortal render portal color and blend
  * 
  * @param portal
  * @param rc
  * @param renderer
  */
  void renderPortal(const Portal& portal, radix::RenderContext& rc,
                    radix::Renderer& renderer);

  /** 
  * @brief Renders a single entity regardless of shading
  * 
  * @param rc
  * @param e
  */
  void renderEntity(radix::RenderContext &rc, const radix::Entity &e);

  /** 
  * @brief Renders the player character using ambient shading
  * 
  * @param rc
  */
  void renderPlayer(radix::RenderContext &rc);

  /**
   * @brief Set the camera in the portal so rendering from that portal is possible
   * @param cam         The camera from which we look at the portal
   * @param dest        The camera to move inside the portal and point in the right direction
   * @param portal      The portal in which to place the camera
   * @param otherPortal The counterpart of the portal
   */
  static void setCameraInPortal(const radix::Camera &cam, radix::Camera &dest,
                const radix::Entity &portal, const radix::Entity &otherPortal);

private:
  radix::Camera* camera;
  double*        dtime;
  double         time = 0.0;
};

} /* namespace glPortal */

#endif /* GAME_RENDERER_HPP */
