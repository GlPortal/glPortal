#ifndef UIRENDERER_HPP
#define UIRENDERER_HPP

#include <string>
#include <engine/renderer/Renderer.hpp>


namespace glPortal {
class Renderer;
  
class UiRenderer {
public:
  static void render(Renderer &renderer);
};

} /* namespace glPortal */

#endif /* UIRENDERER_HPP */
