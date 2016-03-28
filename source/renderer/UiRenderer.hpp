#ifndef UIRENDERER_HPP
#define UIRENDERER_HPP

namespace radix {
  struct RenderContext;
  class Viewport;
}

namespace glPortal {

class World;
class Screen;

class UiRenderer {
public:
  static void render(radix::RenderContext &rc, World &world);
  static void renderScreen(radix::RenderContext &rc, World &world, Screen &scr);
  static void renderHand(radix::RenderContext &rc, World &world);
};

} /* namespace glPortal */

#endif /* UIRENDERER_HPP */
