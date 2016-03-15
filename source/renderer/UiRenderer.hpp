#ifndef UIRENDERER_HPP
#define UIRENDERER_HPP

namespace radix {
  class Renderer;
  class Viewport;
  class World;
}

namespace glPortal {

class UiRenderer {
public:
  static void render(radix::Renderer &renderer, radix::World &world);
  static void renderScreen(radix::Renderer &renderer, radix::World &world);
  static void renderHand(radix::Renderer &renderer, radix::World &world);
};

} /* namespace glPortal */

#endif /* UIRENDERER_HPP */
