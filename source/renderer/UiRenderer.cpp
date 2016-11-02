#include <glPortal/renderer/UiRenderer.hpp>
#include <glPortal/Game.hpp>

#include <epoxy/gl.h>

#include <radix/model/MeshLoader.hpp>
#include <radix/shader/ShaderLoader.hpp>
#include <radix/material/MaterialLoader.hpp>

#include "../Version.hpp"

using namespace radix;

namespace glPortal {

UiRenderer::UiRenderer(World &w, radix::Renderer &ren) :
    SubRenderer(w, ren) {

}

void UiRenderer::render() {
  glDepthMask(GL_FALSE);

  renderer.getViewport()->getSize(&viewportWidth, &viewportHeight);

  initCamera();

  // Hand
  renderImage(Vector3f(viewportWidth - 400, 250, -50), Vector3f(960, 540, 1), "hand.png");

  // Crosshair
  renderImage(Vector3f(viewportWidth / 2, viewportHeight / 2, -10), Vector3f(80, 80, 1), "Reticle.png");

  Text glPortalTitle;
  glPortalTitle.font     = "Pacaya";
  glPortalTitle.size     = 1.5f;
  glPortalTitle.content  = "GlPortal";
  glPortalTitle.color    = Vector4f(1, 1, 1, 1);
  glPortalTitle.position = Vector3f(25, viewportHeight - 95, -20);
  renderer.renderText(*renderContext.get(), glPortalTitle);

  Text fpsCounter;
  fpsCounter.font     = "Pacaya";
  fpsCounter.size     = 0.5f;
  fpsCounter.content  = std::string("FPS: ") + std::to_string(Game::fps.getFps());
  fpsCounter.color    = Vector4f(1, 1, 1, 1);
  fpsCounter.position = Vector3f(10, viewportHeight - 25, -20);
  renderer.renderText(*renderContext.get(), fpsCounter);

  Text version;
  version.font     = "Pacaya";
  version.size     = 0.5f;
  version.content  = std::string("Early testing build: ")  + GAME_VERSION;
  version.color    = Vector4f(1, 1, 1, 1);
  version.position = Vector3f(10, viewportHeight - 45, -20);
  renderer.renderText(*renderContext.get(), version);

  renderContext->popCamera();
  glDepthMask(GL_TRUE);
}

void UiRenderer::renderImage(radix::Vector3f position, radix::Vector3f scale, std::string path) {
  Matrix4f matrix;
  matrix.translate(position);
  matrix.scale(scale);

  const Mesh &mesh = MeshLoader::getMesh("GUIElement.obj");
  const Material &material = MaterialLoader::fromTexture(path);

  Shader &sh = ShaderLoader::getShader("unshaded.frag");
  renderer.renderMesh(*renderContext.get(), sh, matrix, mesh, material);
  sh.release();
}

} /* namespace glPortal */
