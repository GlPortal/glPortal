#include "TerminalRenderer.hpp"
#include "Renderer.hpp"
#include <assets/model/MeshLoader.hpp>
#include <assets/texture/TextureLoader.hpp>
#include <assets/text/FontLoader.hpp>
#include <assets/shader/ShaderLoader.hpp>
#include <assets/material/MaterialLoader.hpp>
#include <Input.hpp>
#include <cstdio>
#include <vector>
#include <string>

namespace glPortal {

void TerminalRenderer::render(Renderer &renderer) {
  if (not renderer.getScene()->terminal->enabled) {
    return;
  }
  
  int vpWidth, vpHeight;
  renderer.getViewport()->getSize(&vpWidth, &vpHeight);
  Camera camera;
  camera.setOrthographic();
  camera.setBounds(0, vpWidth, 0, vpHeight);
  Matrix4f widget;
  widget.translate(Vector3f(vpWidth/2, vpHeight-100, -5));
  widget.scale(Vector3f(vpWidth, 200, 1));
  const Mesh &mesh = MeshLoader::getMesh("GUIElement.obj");
  Shader &sh = ShaderLoader::getShader("color.frag");
  Vector4f screenBackgroundColor = renderer.getScene()->terminal->backgroundColor;
  glUseProgram(sh.handle);
  glUniform4f(sh.uni("color"),
              screenBackgroundColor.x,
              screenBackgroundColor.y,
              screenBackgroundColor.z,
              screenBackgroundColor.w);
  renderer.renderMesh(camera, sh, widget, mesh, nullptr);
  renderer.setFontColor(renderer.getScene()->screen->textColor);
  renderer.setFontSize(1);
  renderer.renderText(camera,
                      Input::getCharBuffer(),
                      Vector3f(0, vpHeight-30, -1));
}

} /* namespace glPortal */
