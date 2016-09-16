#include "UiRenderer.hpp"

#include <string>
#include <iostream>

#include <epoxy/gl.h>

#include <radix/renderer/Renderer.hpp>
#include <radix/model/MeshLoader.hpp>
#include <radix/shader/ShaderLoader.hpp>
#include <radix/material/MaterialLoader.hpp>
#include <radix/Viewport.hpp>

#include "../Screen.hpp"
#include "../Game.hpp"
#include "../Version.hpp"

using namespace radix;

namespace glPortal {

UiRenderer::UiRenderer(glPortal::World& w, radix::Renderer& ren) :
  world(w),
  renderer(ren),
  camera(nullptr),
  viewportWidth(0), viewportHeight(0) {
  renderContext = std::make_unique<RenderContext>(ren);
}

void UiRenderer::render() {
  glDepthMask(GL_FALSE);

  renderer.getViewport()->getSize(&viewportWidth, &viewportHeight);

  initCamera();

  // Hand
  renderImage(Vector3f(viewportWidth - 400, 250, -50), Vector3f(960, 540, 1), "hand.png");

  // Crosshair
  renderImage(Vector3f(viewportWidth / 2, viewportHeight / 2, -10), Vector3f(80, 80, 1), "Reticle.png");

  // Title
  renderer.setFont("Pacaya", 1.5f);
  renderer.renderText(*renderContext.get(), "GlPortal", Vector3f(25, viewportHeight - 95, -20));

  // FPS counter
  renderer.setFont("Pacaya", 0.5f);
  renderer.renderText(*renderContext.get(),
                      std::string("FPS: ") + std::to_string(Game::fps.getFps()),
                      Vector3f(10, viewportHeight - 25, -20));

  // Version
  renderer.renderText(*renderContext.get(),
                      std::string("Early testing build: ")  + GAME_VERSION,
                      Vector3f(10, viewportHeight - 45, -20));

  renderContext->popCamera();
  glDepthMask(GL_TRUE);
}

void UiRenderer::initCamera() {
  camera = std::make_unique<Camera>();
  camera->setOrthographic();
  camera->setBounds(0, viewportWidth, 0, viewportHeight);
  renderContext->pushCamera(*camera.get());
}

void UiRenderer::renderImage(radix::Vector3f position, radix::Vector3f scale, std::string path) {
  Matrix4f matrix;
  matrix.translate(position);
  matrix.scale(scale);

  const Mesh &mesh = MeshLoader::getMesh("GUIElement.obj");
  const Material &material = MaterialLoader::fromTexture(path);

  Shader &sh = ShaderLoader::getShader("unshaded.frag");
  renderer.renderMesh(*renderContext.get(), sh, matrix, mesh, material);
}

} /* namespace glPortal */
