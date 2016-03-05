#ifndef TERMINAL_RENDERER_HPP
#define TERMINAL_RENDERER_HPP

#include <string>
#include <engine/renderer/Renderer.hpp>
#include <assets/shader/Shader.hpp>
#include <Game.hpp>

namespace glPortal {
class Renderer;
class Viewport;
class Entity;
class Portal;
class Scene;
class Texture;
class Font;

class TerminalRenderer {
public:
  static void render(Renderer &renderer);
};

} /* namespace glPortal */

#endif /* TERMINAL_RENDERER_HPP */
