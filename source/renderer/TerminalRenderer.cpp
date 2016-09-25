#include "TerminalRenderer.hpp"

#include <cstdio>
#include <vector>
#include <string>

#include <epoxy/gl.h>

#include <radix/renderer/Renderer.hpp>
#include <radix/model/MeshLoader.hpp>
#include <radix/texture/TextureLoader.hpp>
#include <radix/text/FontLoader.hpp>
#include <radix/shader/ShaderLoader.hpp>
#include <radix/material/MaterialLoader.hpp>
#include <radix/Viewport.hpp>
#include <radix/World.hpp>

#include "../component/Terminal.hpp"

using namespace radix;

namespace glPortal {

void TerminalRenderer::render(RenderContext &rc, World &world) {
  int vpWidth, vpHeight;
  Renderer &renderer = rc.renderer;
  renderer.getViewport()->getSize(&vpWidth, &vpHeight);
  Camera camera;
  camera.setOrthographic();
  camera.setBounds(0, vpWidth, 0, vpHeight);
  rc.pushCamera(camera);
  Matrix4f widget;
  widget.translate(Vector3f(vpWidth/2, vpHeight-100, -5));
  widget.scale(Vector3f(vpWidth, 200, 1));
  const Mesh &mesh = MeshLoader::getMesh("GUIElement.obj");
  Shader &sh = ShaderLoader::getShader("color.frag");
  Terminal &term = world.entityAliases.at("activeTerminal")->getComponent<Terminal>();
  Vector4f screenBackgroundColor = term.backgroundColor;
  sh.bind();
  glUniform4f(sh.uni("color"),
              screenBackgroundColor.x,
              screenBackgroundColor.y,
              screenBackgroundColor.z,
              screenBackgroundColor.w);
  renderer.renderMesh(rc, sh, widget, mesh, nullptr);
  renderer.setFontColor(term.textColor);
  renderer.setFontSize(1);
  renderer.renderText(rc,
                      world.input.getCharBuffer(),
                      Vector3f(0, vpHeight-30, -1));
  rc.popCamera();
}

} /* namespace glPortal */
