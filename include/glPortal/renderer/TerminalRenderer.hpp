#ifndef TERMINAL_RENDERER_HPP
#define TERMINAL_RENDERER_HPP

#include <string>

namespace radix {
  struct RenderContext;
  class World;
}

namespace glPortal {

class TerminalRenderer {
public:
  static void render(radix::RenderContext &rc, radix::World &world);
};

} /* namespace glPortal */

#endif /* TERMINAL_RENDERER_HPP */
