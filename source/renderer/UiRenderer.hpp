#ifndef UIRENDERER_HPP
#define UIRENDERER_HPP

#include <memory>
#include <string>

#include <radix/core/math/Vector3f.hpp>
#include <radix/renderer/SubRenderer.hpp>

namespace radix {
  struct RenderContext;
  class Camera;
  class Renderer;
}

namespace glPortal {

class World;

class UiRenderer : public radix::SubRenderer{
public:
  UiRenderer(World& w, radix::Renderer& ren);

  void render();

  void renderImage(radix::Vector3f position, radix::Vector3f scale, std::string path);
};

} /* namespace glPortal */

#endif /* UIRENDERER_HPP */
