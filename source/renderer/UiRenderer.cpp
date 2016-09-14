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
  renderImage(Vector3f(viewportWidth-400, 250, -50), Vector3f(960, 540, 1), "hand.png");

  // Crosshair
  renderImage(Vector3f(viewportWidth/2, viewportHeight/2, -10), Vector3f(80, 80, 1), "Reticle.png");

  // Title
  renderer.setFont("Pacaya", 1.5f);
  renderer.renderText(*renderContext.get(),"GlPortal", Vector3f(25, viewportHeight - 95, -20));

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


void UiRenderer::render(RenderContext &rc, World &world) {
  glDepthMask(GL_FALSE);
  renderHand(rc, world);
  int vpWidth, vpHeight;
  Renderer &renderer = rc.renderer;
  std::cout << std::addressof(rc.renderer) << std::endl;
  renderer.getViewport()->getSize(&vpWidth, &vpHeight);

  Camera camera;
  camera.setOrthographic();
  camera.setBounds(0, vpWidth, 0, vpHeight);
  rc.pushCamera(camera);

  Matrix4f crosshairMtx;
  crosshairMtx.translate(Vector3f(vpWidth/2, vpHeight/2, -10));
  crosshairMtx.scale(Vector3f(80, 80, 1));
  Matrix4f widget;
  widget.translate(Vector3f(vpWidth/2+12, vpHeight-20, -2));
  widget.scale(Vector3f(380, 80, 1));

  const Mesh &mesh = MeshLoader::getMesh("GUIElement.obj");
  const Material &mat = MaterialLoader::fromTexture("Reticle.png");

  Shader &sh = ShaderLoader::getShader("unshaded.frag");
  renderer.renderMesh(rc, sh, crosshairMtx, mesh, mat);

  // Title
  renderer.setFont("Pacaya", 1.5f);
  renderer.renderText(rc, "GlPortal", Vector3f(25, vpHeight - 95, -20));

  // FPS counter
  renderer.setFont("Pacaya", 0.5f);
  renderer.renderText(rc,
                      std::string("FPS: ") + std::to_string(Game::fps.getFps()),
                      Vector3f(10, vpHeight - 25, -20));
  renderer.renderText(rc,
                      std::string("Early testing build: ")  + GAME_VERSION,
                      Vector3f(10, vpHeight - 45, -20));

  if (world.activeScreen) {
    renderScreen(rc, world, *world.activeScreen);
  }
  rc.popCamera();
  glDepthMask(GL_TRUE);
}

void UiRenderer::renderScreen(RenderContext &rc, World &world, Screen &scr) {
  // TODO: restore
  int vpWidth, vpHeight;
  Renderer &renderer = rc.renderer;
  renderer.getViewport()->getSize(&vpWidth, &vpHeight);

  Camera camera;
  camera.setOrthographic();
  camera.setBounds(0, vpWidth, 0, vpHeight);
  rc.pushCamera(camera);

  Matrix4f widget;
  widget.translate(Vector3f(vpWidth/2, vpHeight/2, -5));
  widget.scale(Vector3f(vpWidth, vpHeight, 1));

  const Mesh &mesh = MeshLoader::getMesh("GUIElement.obj");

  Shader &sh = ShaderLoader::getShader("color.frag");

  Vector4f screenBackgroundColor = scr.backgroundColor;
  glUseProgram(sh.handle);
  glUniform4f(sh.uni("color"),
              screenBackgroundColor.r,
              screenBackgroundColor.g,
              screenBackgroundColor.b,
              screenBackgroundColor.a);

  renderer.renderMesh(rc, sh, widget, mesh, nullptr);
  renderer.setFontColor(scr.textColor);
  renderer.setFontSize(4);
  renderer.renderText(rc, scr.title,
    Vector3f((vpWidth/2)-(renderer.getTextWidth(scr.title)/2),
             vpHeight/2, -1));
  renderer.setFontSize(0.5);
  renderer.renderText(rc, scr.text,
    Vector3f((vpWidth/2)-(renderer.getTextWidth(scr.text)/2),
             (vpHeight/2)-100, -1));
  rc.popCamera();
}

void UiRenderer::renderHand(RenderContext &rc, World &world) {
  int vpWidth, vpHeight;
  rc.renderer.getViewport()->getSize(&vpWidth, &vpHeight);
  Camera camera;
  camera.setOrthographic();
  camera.setBounds(0, vpWidth, 0, vpHeight);
  rc.pushCamera(camera);
  Matrix4f widget;
  widget.translate(Vector3f(vpWidth-400, 250, -50));
  widget.scale(Vector3f(960, 540, 1));
  const Mesh &mesh = MeshLoader::getMesh("GUIElement.obj");
  const Material &mat = MaterialLoader::fromTexture("hand.png");
  Shader &sh = ShaderLoader::getShader("unshaded.frag");
  //glUniform4f(sh.uni("color"), 0, 0, 0, scr.alpha);
  rc.renderer.renderMesh(rc, sh, widget, mesh, mat);
  rc.popCamera();
}

} /* namespace glPortal */
