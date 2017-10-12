#ifndef PORTAL_RENDERER_HPP
#define PORTAL_RENDERER_HPP

#include <radix/core/math/Vector3f.hpp>
#include <radix/renderer/SubRenderer.hpp>

#include <glPortal/Portal.hpp>

namespace radix {
  struct RenderContext;
  class Camera;
  class Renderer;
  class Entity;
}

namespace glPortal {

class World;

class PortalRenderer : public radix::SubRenderer{
public:
  PortalRenderer(World& w, radix::Renderer& ren, radix::Camera* cam);

  void render();
  
  void renderPortalOverlay(const Portal &portal);

private:
  radix::Camera* camera;
};

} /* namespace glPortal */

#endif
