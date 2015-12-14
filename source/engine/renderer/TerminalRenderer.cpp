#include "TerminalRenderer.hpp"
#include "Renderer.hpp"
#include <assets/model/MeshLoader.hpp>
#include <assets/texture/TextureLoader.hpp>
#include <assets/text/FontLoader.hpp>
#include <assets/shader/ShaderLoader.hpp>
#include <assets/material/MaterialLoader.hpp>

#include <cstdio>
#include <vector>
#include <string>

namespace glPortal {
void TerminalRenderer::render(Renderer &renderer) {
  if (not renderer.scene->terminal->enabled) {
    return;
  }
  
  int vpWidth  = renderer.vpWidth;
  int vpHeight = renderer.vpHeight;
  Camera camera;
  camera.setOrthographic();
  camera.setBounds(0, vpWidth, 0, vpHeight);
  Matrix4f widget;
  widget.translate(Vector3f(vpWidth/2, vpHeight-100, -5));
  widget.scale(Vector3f(vpWidth, 200, 1));
  const Mesh &mesh = MeshLoader::getMesh("GUIElement.obj");
  renderer.setShader(&ShaderLoader::getShader("color.frag"));
  Vector4f screenBackgroundColor = renderer.scene->terminal->backgroundColor;
  glUniform4f(renderer.shader->uni("color"),
              screenBackgroundColor.x,
              screenBackgroundColor.y,
              screenBackgroundColor.z,
              screenBackgroundColor.w);
  renderer.renderMesh(camera, widget, mesh, nullptr);
  renderer.setFontColor(renderer.scene->screen->textColor);
  renderer.setFontSize(1);
  renderer.renderText(camera,
                      "Foo",
                      Vector3f(0,vpHeight-30, -1));
}
} /* namespace glPortal */
