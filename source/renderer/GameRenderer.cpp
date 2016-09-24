#include "GameRenderer.hpp"

#include <epoxy/gl.h>

#include <radix/renderer/Renderer.hpp>
#include <radix/renderer/RenderContext.hpp>
#include <radix/Viewport.hpp>
#include <radix/Camera.hpp>

using namespace radix;

namespace glPortal {

GameRenderer::GameRenderer(glPortal::World& w, radix::Renderer& ren) :
  world(w),
  renderer(ren),
  camera(nullptr),
  viewportWidth(0), viewportHeight(0) {
  rc = std::make_unique<RenderContext>(ren);
}

void GameRenderer::render(double dtime, const Camera &cam) {
  renderer.render(dtime, cam);

  //
  //
  // initCamera();
  //
  // glClear(GL_COLOR_BUFFER_BIT);
  //
  // renderContext->popCamera();
  time += dtime;
  renderer.getViewport()->getSize(&viewportWidth, &viewportHeight);

  glDepthMask(GL_TRUE);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);

  glClearColor(0, 0, 0, 1.0);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  Camera camera = cam;
  camera.setPerspective();
  camera.setAspect((float) viewportWidth / viewportHeight);

  rc->projStack.resize(1);
  camera.getProjMatrix(rc->projStack.back());

  rc->viewStack.resize(1);
  camera.getViewMatrix(rc->viewStack.back());

  rc->invViewStack.resize(1);
  camera.getInvViewMatrix(rc->invViewStack.back());

  rc->viewFramesStack.clear();

  rc->projDirty = rc->viewDirty = true;

  renderer.renderScene(*rc);
}

void GameRenderer::initCamera() {
  camera = std::make_unique<Camera>();
  camera->setOrthographic();
  camera->setBounds(0, viewportWidth, 0, viewportHeight);
  rc->pushCamera(*camera.get());
}

} /* namespace glPortal */
