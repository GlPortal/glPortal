#ifndef UIRENDERER_HPP
#define UIRENDERER_HPP

namespace radix {
  class Renderer;
  class Viewport;
}

namespace glPortal {

class World;
class Screen;

class UiRenderer {
public:
  static void render(radix::Renderer &renderer, World &world);
  static void renderScreen(radix::Renderer &renderer, World &world, Screen &scr);
  static void renderHand(radix::Renderer &renderer, World &world);
};

} /* namespace glPortal */

#endif /* UIRENDERER_HPP */
