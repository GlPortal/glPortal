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
  crosshairMtx.translate(Vector3f(vpWidth/2, vpHeight/2, -10));
  crosshairMtx.scale(Vector3f(80, 80, 1));
  Matrix4f widget;
  widget.translate(Vector3f(vpWidth/2+12, vpHeight-20, -2));
  widget.scale(Vector3f(380, 80, 1));

  const Mesh &mesh = MeshLoader::getMesh("GUIElement.obj");
  const Material &mat = MaterialLoader::fromTexture("Reticle.png");

  renderer.setShader(&ShaderLoader::getShader("unshaded.frag"));
  renderer.renderMesh(camera, crosshairMtx, mesh, mat);

  // Title
  renderer.setFont("Pacaya", 1.5f);
  renderer.renderText(camera, "GlPortal", Vector3f(25, vpHeight - 75, -20));

  // FPS counter
  renderer.setFont("Pacaya", 0.5f);
  renderer.renderText(camera,
                      std::string("FPS: ") + std::to_string(Game::fps.getFps()),
                      Vector3f(10, vpHeight - 25, -20));
  if (renderer.scene->screen->enabled){
    renderScreen(renderer);
  }
  renderHand(renderer);
}

void UiRenderer::renderScreen(Renderer &renderer) {
  int vpWidth  = renderer.vpWidth;
  int vpHeight = renderer.vpHeight;
  Camera camera;
  camera.setOrthographic();
  camera.setBounds(0, vpWidth, 0, vpHeight);
  Matrix4f widget;
  widget.translate(Vector3f(vpWidth/2, vpHeight/2, -5));
  widget.scale(Vector3f(vpWidth, vpHeight, 1));
  const Mesh &mesh = MeshLoader::getMesh("GUIElement.obj");
  renderer.setShader(&ShaderLoader::getShader("color.frag"));
  Vector4f screenBackgroundColor = renderer.scene->screen->backgroundColor;
  glUniform4f(renderer.shader->uni("color"),
              screenBackgroundColor.x,
              screenBackgroundColor.y,
              screenBackgroundColor.z,
              screenBackgroundColor.w);
  renderer.renderMesh(camera, widget, mesh, nullptr);
  renderer.setFontColor(renderer.scene->screen->textColor);
  renderer.setFontSize(4);
  renderer.renderText(camera,
                      renderer.scene->screen->title,
                      Vector3f((vpWidth/2)-(renderer.getTextWidth(renderer.scene->screen->title)/2),
                               vpHeight/2, -1));
  renderer.setFontSize(0.5);
  renderer.renderText(camera,
                      renderer.scene->screen->text,
                      Vector3f((vpWidth/2)-(renderer.getTextWidth(renderer.scene->screen->text)/2),
                               (vpHeight/2)-100, -1));
}

void UiRenderer::renderHand(Renderer &renderer) {
  int vpWidth, vpHeight;
  renderer.viewport->getSize(&vpWidth, &vpHeight);
  Camera camera;
  camera.setOrthographic();
  camera.setBounds(0, vpWidth, 0, vpHeight);
  Matrix4f widget;
  widget.translate(Vector3f(vpWidth-400, 250, -50));
  widget.scale(Vector3f(960, 540, 1));
  const Mesh &mesh = MeshLoader::getMesh("GUIElement.obj");
  const Material &mat = MaterialLoader::fromTexture("hand.png");
  renderer.setShader(&ShaderLoader::getShader("unshaded.frag"));
  glUniform4f(renderer.shader->uni("color"), 0, 0, 0, renderer.scene->screen->alpha);
  renderer.renderMesh(camera, widget, mesh, mat);
}
} /* namespace glPortal */
