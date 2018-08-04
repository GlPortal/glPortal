#ifndef GAME_RENDERER_HPP
#define GAME_RENDERER_HPP

#include <array>
#include <radix/renderer/SubRenderer.hpp>

namespace radix {
struct RenderContext;
class Camera;
class Renderer;
class Entity;
struct FrameBuffer;
}  // namespace radix

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
  GameRenderer(World& w, radix::Renderer& ren, radix::Camera* cam,
               double* ptime);

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
  void renderScene(radix::RenderContext& renderContext);

  /**
   * @brief
   *
   * @param renderContext
   */
  void renderDebugView(radix::RenderContext& renderContext);

  /**
   * @brief
   *
   * @param rc
   */
  void renderViewFrames(radix::RenderContext& rc);

  /**
   * @brief
   *
   * @param rc
   */
  void renderViewFrameStencil(radix::RenderContext& rc);

  /**
   * @brief Renders all the entities in the scene regardless of shading
   *
   * @param rc
   */
  void renderEntities(radix::RenderContext& rc);

  /**
   * @brief renderPortals render both portals color and blend
   *
   * @param rc
   */
  void renderPortals(radix::RenderContext& rc);

  // GL_ALWAYS  0x0207
  // GL_REPLACE 0x1E01
  void testPortalStencil(const Portal& portal, radix::RenderContext& renderContext,
      const int stencilIndex = 1,
      const unsigned int stencilFuncType = 0x0207,
      const unsigned int stencilSuccess  = 0x1E01);

  void renderSceneFromPortal(const radix::Camera& src, const Portal& portal1,
                             const Portal& portal2, radix::RenderContext&,
                             const int stencilIndex,
                             const unsigned int occlusionQueryIndex,
                             const unsigned int count);

  /**
   * @brief renderPortal render portal color and blend
   *
   * @param portal
   * @param rc
   */
  void renderStencilPortal(const Portal& portal, radix::RenderContext& rc,
                           const unsigned int query);

  /**
   * @brief Renders a single entity regardless of shading
   *
   * @param rc
   * @param e
   */
  void renderEntity(radix::RenderContext& rc, const radix::Entity& e);

  /**
   * @brief Renders the player character using ambient shading
   *
   * @param rc
   */
  void renderPlayer(radix::RenderContext& rc);

  /**
   * @brief Set the camera in the portal so rendering from that portal is
   * possible
   * @param cam         The camera from which we look at the portal
   * @param dest        The camera to move inside the portal and point in the
   * right direction
   * @param portal      The portal in which to place the camera
   * @param otherPortal The counterpart of the portal
   */
  static void setCameraInPortal(const radix::Camera& cam, radix::Camera& dest,
                                const radix::Entity& portal,
                                const radix::Entity& otherPortal);

  private:
  radix::Camera* camera;
  double* dtime;
  double time = 0.0;
  std::array<unsigned int, 10> occlusionQueryIdx;
};

} /* namespace glPortal */

#endif /* GAME_RENDERER_HPP */
