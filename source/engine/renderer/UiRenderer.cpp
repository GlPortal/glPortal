#include "UiRenderer.hpp"
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

void UiRenderer::render(Renderer &renderer) {
  int vpWidth, vpHeight;
  renderer.viewport->getSize(&vpWidth, &vpHeight);
  Camera camera;
  camera.setOrthographic();
  camera.setBounds(0, vpWidth, 0, vpHeight);
  Matrix4f crosshairMtx;
  crosshairMtx.translate(Vector3f(vpWidth/2, vpHeight/2, -2));
  crosshairMtx.scale(Vector3f(80, 80, 1));
  Matrix4f widget;
  widget.translate(Vector3f(vpWidth/2+12, vpHeight/2+12, -2));
  widget.scale(Vector3f(80, 80, 1));
  
  const Mesh &mesh = MeshLoader::getMesh("GUIElement.obj");
  const Material &mat = MaterialLoader::fromTexture("Reticle.png");
  const Shader &unshaded = ShaderLoader::getShader("unshaded.frag");
  renderer.renderMesh(camera, unshaded, crosshairMtx, mesh, mat);
  renderer.renderColoredMesh(camera, widget);
  // Title
  renderer.setFont("Pacaya", 1.5f);
  renderer.renderText(camera, "GlPortal", 25, vpHeight - 75);

  // FPS counter
  renderer.setFont("Pacaya", 0.5f);
  renderer.renderText(camera, std::string("FPS: ") + std::to_string(Game::fps.getFps()), 10, vpHeight - 25);
}
} /* namespace glPortal */
