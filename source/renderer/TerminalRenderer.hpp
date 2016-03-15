#ifndef TERMINAL_RENDERER_HPP
#define TERMINAL_RENDERER_HPP

#include <string>

#include <radix/renderer/Renderer.hpp>

namespace glPortal {

class TerminalRenderer {
public:
  static void render(radix::Renderer &renderer, radix::World &world);
};

} /* namespace glPortal */

#endif /* TERMINAL_RENDERER_HPP */
