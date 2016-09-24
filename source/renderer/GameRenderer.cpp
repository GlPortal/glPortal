#include "GameRenderer.hpp"

#include <epoxy/gl.h>

#include <radix/renderer/Renderer.hpp>
#include <radix/Viewport.hpp>
#include <radix/Camera.hpp>

using namespace radix;

namespace glPortal {

GameRenderer::GameRenderer(glPortal::World& w, radix::Renderer& ren) :
  world(w),
  renderer(ren),
  camera(nullptr),
  viewportWidth(0), viewportHeight(0) {
  renderContext = std::make_unique<RenderContext>(ren);
}

void GameRenderer::render(double dtime, const Camera &cam) {
  renderer.render(dtime, cam);

  // renderer.getViewport()->getSize(&viewportWidth, &viewportHeight);
  //
  // initCamera();
  //
  // glClear(GL_COLOR_BUFFER_BIT);
  //
  // renderContext->popCamera();
}

void GameRenderer::initCamera() {
  camera = std::make_unique<Camera>();
  camera->setOrthographic();
  camera->setBounds(0, viewportWidth, 0, viewportHeight);
  renderContext->pushCamera(*camera.get());
}

} /* namespace glPortal */
