#include <glPortal/renderer/GameRenderer.hpp>
#include <glPortal/World.hpp>
#include <glPortal/Portal.hpp>
#include <radix/env/Config.hpp>
#include <radix/core/gl/OpenGL.hpp>

#include <radix/renderer/Renderer.hpp>
#include <radix/Viewport.hpp>
#include <radix/entities/Player.hpp>
#include <radix/entities/ViewFrame.hpp>
#include <radix/entities/traits/MeshDrawableTrait.hpp>
#include <radix/data/shader/ShaderLoader.hpp>
#include <radix/data/model/MeshLoader.hpp>
#include <radix/data/material/MaterialLoader.hpp>
#include <radix/util/Profiling.hpp>

#include <radix/simulation/Physics.hpp>
#include <radix/physics/PhysicsDebugDraw.hpp>

#include <radix/core/gl/OpenGL.hpp>


using namespace radix;

struct backupColor {
  explicit backupColor(GLboolean initMask[4]) {
    glGetBooleanv(GL_COLOR_WRITEMASK, mask.data());
    glColorMask(initMask[0], initMask[1], initMask[2], initMask[3]);
  }
  ~backupColor() {
    glColorMask(mask[0], mask[1], mask[2], mask[3]);
  }
  void setMask(GLboolean mask[4]) {
    glColorMask(mask[0], mask[1], mask[2], mask[3]);
  }
protected:
  std::array<GLboolean, 4> mask;
};

struct backupDepth {
  explicit backupDepth(GLboolean initMask) {
    glGetBooleanv(GL_DEPTH_WRITEMASK, &mask);
    glDepthMask(initMask);
  }
  ~backupDepth() {
    glDepthMask(mask);
  }
  void setMask(GLboolean mask) {
    glDepthMask(mask);
  }
protected:
  GLboolean mask;
};

struct backupStencil {
  explicit backupStencil(GLuint initMask) {
    glGetIntegerv(GL_STENCIL_WRITEMASK, &mask);
    glStencilMask(initMask);
  }
  ~backupStencil() {
    glStencilMask(mask);
  }
  void setMask(GLuint mask) {
    glStencilMask(mask);
  }
protected:
  GLint mask;
};

namespace glPortal {

GameRenderer::GameRenderer(glPortal::World &w, radix::Renderer &ren,
                           radix::Camera *cam, double *ptime)
    : SubRenderer(dynamic_cast<radix::World &>(w), ren),
      camera(cam),
      dtime(ptime) {
  glGenQueries(occlusionQueryIdx.size(), occlusionQueryIdx.data());
}

GameRenderer::~GameRenderer() { glDeleteQueries(occlusionQueryIdx.size(), occlusionQueryIdx.data()); }

void GameRenderer::testPortalStencil(const Portal &portal,
                                     RenderContext &renderContext,
                                     const int stencilIndex,
                                     const unsigned int stencilFuncType,
                                     const GLenum stencilSuccess) {
  Matrix4f mtx;
  auto flatShader = ShaderLoader::getShader("whitefill.frag");
  auto portalMesh = portal.stencilMesh;

  glStencilMask(0xFF);
  glDepthMask(GL_FALSE);
  glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

  glStencilOp(GL_KEEP, GL_KEEP, stencilSuccess);
  glStencilFunc(stencilFuncType, stencilIndex, 0xFF);

  portal.getModelMtx(mtx);
  const auto scale = portal.getScaleMult();
  mtx.scale(scale);

  renderer.renderMesh(renderContext, flatShader, mtx, portalMesh, nullptr);

  glDepthMask(GL_TRUE);
  glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
  glStencilMask(0x00);
}

void GameRenderer::renderSceneFromPortal(const Camera &src,
                                         const Portal &portal1,
                                         const Portal &portal2,
                                         RenderContext &renderContext,
                                         const int stencilIndex,
                                         const unsigned int occlusionQueryIndex,
                                         const unsigned int count) {
  if (count > 3) {
    return;
  }

  glClear(GL_DEPTH_BUFFER_BIT);
  glStencilFunc(GL_EQUAL, stencilIndex, 0xFF);
  Camera portalCamera;
  setCameraInPortal(src, portalCamera, portal1, portal2);
  renderContext.pushCamera(portalCamera);

  {
    backupStencil stencil(0x00);
    glBeginConditionalRender(occlusionQueryIdx[occlusionQueryIndex], GL_QUERY_WAIT);
      renderEntities(renderContext);
      renderPlayer(renderContext);
      renderStencilPortal(portal1, renderContext, occlusionQueryIndex + 1);
    glEndConditionalRender();
  }

    testPortalStencil(portal1, renderContext, stencilIndex, GL_EQUAL, GL_INCR);

    renderSceneFromPortal(portalCamera, portal1, portal2,
        renderContext, stencilIndex+1, occlusionQueryIndex+1, count+1);
  renderContext.popCamera();
}

void GameRenderer::render() {
  PROFILER_BLOCK("GameRenderer::render", profiler::colors::Green200);
  time += *dtime;
  renderer.getViewport()->getSize(&viewportWidth, &viewportHeight);

  glViewport(0, 0, viewportWidth, viewportHeight);

  PROFILER_BLOCK("GL cleanup");
  glDepthMask(GL_TRUE);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);

  glClearColor(0, 0, 0, 1.0);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  PROFILER_END_BLOCK;

  PROFILER_BLOCK("Rendering context setup");
  camera->setPerspective();
  camera->setAspect(static_cast<float>(viewportWidth) / viewportHeight);

  renderContext->projStack.resize(1);
  camera->getProjMatrix(renderContext->projStack.back());

  renderContext->viewStack.resize(1);
  camera->getViewMatrix(renderContext->viewStack.back());

  renderContext->invViewStack.resize(1);
  camera->getInvViewMatrix(renderContext->invViewStack.back());

  renderContext->viewFramesStack.clear();

  renderContext->projDirty = renderContext->viewDirty = true;
  PROFILER_END_BLOCK;

  renderScene(*renderContext);
}

void GameRenderer::renderScene(RenderContext &renderContext) {
  PROFILER_BLOCK("GameRenderer::renderScene", profiler::colors::Green300);
  if (renderContext.viewFramesStack.size() > renderContext.viewStackMaxDepth) {
    return;
  }

  RectangleI scissor{0, 0, 0, 0};
  if (not renderContext.viewFramesStack.empty()) {
    const RenderContext::ViewFrameInfo &vfi = renderContext.getViewFrame();
    // Don't render further if computed clip rect is zero-sized
    if (not renderer.clipViewFrame(renderContext, vfi.first, vfi.second,
                                   scissor)) {
      return;
    }
  }

  glClear(GL_DEPTH_BUFFER_BIT);

  renderViewFrames(renderContext);

  if (not renderContext.viewFramesStack.empty()) {
    glScissor(scissor.x, scissor.y, scissor.w, scissor.h);
    renderViewFrameStencil(renderContext);
  }

  renderEntities(renderContext);
  renderPortals(renderContext);

  glClear(GL_DEPTH_BUFFER_BIT);
  if (world.getConfig().isDebugViewEnabled()) {
    renderDebugView(renderContext);
  }
}

void GameRenderer::renderDebugView(RenderContext &renderContext) {
  btIDebugDraw *iDbgDraw =
      world.simulations.findFirstOfType<simulation::Physics>().getDebugDraw();
  auto dbgDraw = dynamic_cast<PhysicsDebugDraw *>(iDbgDraw);
  if (dbgDraw != nullptr) {
    world.simulations.findFirstOfType<radix::simulation::Physics>()
        .getPhysicsWorld()
        .debugDrawWorld();
    dbgDraw->render(renderContext);
  }
}

void GameRenderer::renderViewFrames(RenderContext &rc) {
  PROFILER_BLOCK("GameRenderer::renderViewFrames");
  GLboolean save_stencil_test;
  glGetBooleanv(GL_STENCIL_TEST, &save_stencil_test);
  GLboolean save_scissor_test;
  glGetBooleanv(GL_SCISSOR_TEST, &save_scissor_test);

  glEnable(GL_STENCIL_TEST);
  glEnable(GL_SCISSOR_TEST);
  for (Entity &entity : world.entityManager) {
    auto vf = dynamic_cast<entities::ViewFrame *>(&entity);
    if (vf != nullptr) {
      Matrix4f inMat;
      entity.getModelMtx(inMat);
      Matrix4f outMat;
      outMat.translate(vf->view.getPosition());
      outMat.rotate(vf->view.getOrientation());
      Matrix4f frameView = renderer.getFrameView(rc.getView(), inMat, outMat);
      rc.pushViewFrame(
          RenderContext::ViewFrameInfo(vf->mesh, entity.transform()));
      rc.pushView(frameView);
      renderScene(rc);
      rc.popView();
      rc.popViewFrame();
    }
  }
  if (save_stencil_test == 0u) {
    glDisable(GL_STENCIL_TEST);
  }
  if (save_scissor_test == 0u) {
    glDisable(GL_SCISSOR_TEST);
  }

  // Draw portal in the depth buffer so they are not overwritten
  glClear(GL_DEPTH_BUFFER_BIT);

  std::array<GLboolean, 4> save_color_mask{};
  GLboolean save_depth_mask;
  glGetBooleanv(GL_COLOR_WRITEMASK, save_color_mask.data());
  glGetBooleanv(GL_DEPTH_WRITEMASK, &save_depth_mask);
  glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
  glDepthMask(GL_TRUE);
  Shader shader = ShaderLoader::getShader("whitefill.frag");
  Matrix4f modelMtx;
  for (auto &view : rc.viewFramesStack) {
    renderer.renderMesh(rc, shader, modelMtx, view.first, nullptr);
  }
  shader.release();
  glColorMask(save_color_mask[0], save_color_mask[1], save_color_mask[2],
              save_color_mask[3]);
  glDepthMask(save_depth_mask);
}

void GameRenderer::renderViewFrameStencil(RenderContext &rc) {
  PROFILER_BLOCK("GameRenderer::renderViewFrameStencil");

  GLboolean color[4] = {GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE};
  backupColor colorBackup(color);
  backupDepth depthBackup(GL_FALSE);
  glStencilFunc(GL_NEVER, 0, 0xFF);
  glStencilOp(GL_INCR, GL_KEEP, GL_KEEP);  // draw 1s on test fail (always)
  glClear(GL_STENCIL_BUFFER_BIT);          // needs mask=0xFF

  rc.pushView(rc.viewStack[0]);
  Shader shader = ShaderLoader::getShader("whitefill.frag");
  Matrix4f modelMtx;
  rc.viewFramesStack.back().second.getModelMtx(modelMtx);
  renderer.renderMesh(rc, shader, modelMtx, rc.viewFramesStack.back().first,
                      nullptr);
  rc.popView();

  for (size_t i = 1; i < rc.viewStack.size() - 1;
       i++) {  // -1 to ignore last view
    // Increment intersection for current portal
    glStencilFunc(GL_EQUAL, 0, 0xFF);
    glStencilOp(GL_INCR, GL_KEEP, GL_KEEP);  // draw 1s on test fail (always)
    renderer.renderMesh(rc, shader, modelMtx, rc.viewFramesStack.back().first,
                        nullptr);

    // Decremental outer portal -> only sub-portal intersection remains
    glStencilFunc(GL_NEVER, 0, 0xFF);
    glStencilOp(GL_DECR, GL_KEEP, GL_KEEP);  // draw 1s on test fail (always)
    rc.pushView(rc.viewStack[i - 1]);
    renderer.renderMesh(rc, shader, modelMtx, rc.viewFramesStack.back().first,
                        nullptr);
  }
  shader.release();

  glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
  /* Fill 1 or more */
  glStencilFunc(GL_LEQUAL, 1, 0xFF);
}

void GameRenderer::renderEntities(RenderContext &rc) {
  for (Entity &e : world.entityManager) {
    if (dynamic_cast<const entities::MeshDrawableTrait *>(&e) != nullptr) {
      renderEntity(rc, e);
    }
  }
}

void GameRenderer::renderPortals(RenderContext &renderContext) {
  auto index = world.entityPairs.find("portalPairs");
  if (index == world.entityPairs.end() || index->second.empty()) {
    return;
  }

  auto &portals = index->second.back();
  Portal *cam1  = dynamic_cast<Portal *>(portals.first);
  Portal *cam2  = dynamic_cast<Portal *>(portals.second);

  // backup stencil buffer
  GLboolean save_stencil_test;
  glGetBooleanv(GL_STENCIL_TEST, &save_stencil_test);
  glEnable(GL_STENCIL_TEST);

  int portal1QueryIndex = 0;
  int portal2QueryIndex = 5;
  renderStencilPortal(*cam1, renderContext, portal1QueryIndex);
  renderStencilPortal(*cam2, renderContext, portal2QueryIndex);

  // TODO
  //GLint portal1Query = 0, portal2Query = 0;
  //glGetQueryObjectiv(GL_QUERY_RESULT_AVAILABLE, occlusionQueryIdx[0], &portal1Query);
  //glGetQueryObjectiv(GL_QUERY_RESULT_AVAILABLE, occlusionQueryIdx[1], &portal2Query);

  if (cam1->open && cam2->open) {
    glClearStencil(0);
    glClear(GL_STENCIL_BUFFER_BIT);

    testPortalStencil(*cam1, renderContext, 1);
    testPortalStencil(*cam2, renderContext, 4);

    renderSceneFromPortal(*camera, *cam1, *cam2, renderContext, 1, portal1QueryIndex, 0);
    renderSceneFromPortal(*camera, *cam2, *cam1, renderContext, 4, portal2QueryIndex, 0);
  }
  // restore stencil buffer
  glStencilMask(0xFF);
  glStencilFunc(GL_ALWAYS, 0, -1);
  glClear(GL_STENCIL_BUFFER_BIT);
  if (save_stencil_test != 0u) {
    glDisable(GL_STENCIL_TEST);
  }
}

void GameRenderer::renderStencilPortal(const Portal &portal, RenderContext &rc,
                                        const GLuint query) {
  if(not portal.open) {
    return;
  }

  backupDepth depthBackup(0x00);
  backupDepth depthDepth(GL_FALSE);

  auto &mesh = portal.overlayMesh;
  auto &mat  = portal.overlayTex;

  Matrix4f mtx;
  mtx.translate(portal.getPosition());
  mtx.rotate(portal.getOrientation());

  // TODO Vector3f(0.05f, 1.05f, 0.05f);
  const auto scale = portal.getScale() * portal.getScaleMult();
  mtx.scale(scale + Vector3f(0.05f, 0.05f, 0.05f));

  auto &shader = ShaderLoader::getShader("unshaded.frag");

  glBeginQuery(GL_SAMPLES_PASSED, this->occlusionQueryIdx[query]);
  renderer.renderMesh(rc, shader, mtx, mesh, mat);
  glEndQuery(GL_SAMPLES_PASSED);

  shader.release();
}

void GameRenderer::renderEntity(RenderContext &rc, const Entity &e) {
  PROFILER_BLOCK("GameRenderer::renderEntity", profiler::colors::Green400);
  const auto &drawable = dynamic_cast<const entities::MeshDrawableTrait &>(e);
  Matrix4f mtx;
  e.getModelMtx(mtx);

  if (drawable.material.fancyname.compare("Metal tiles .5x") == 0) {
    Shader &metal = ShaderLoader::getShader("metal.frag");
    renderer.renderMesh(rc, metal, mtx, drawable.mesh, drawable.material);
    metal.release();
  } else {
    Shader &diffuse = ShaderLoader::getShader("diffuse.frag");
    renderer.renderMesh(rc, diffuse, mtx, drawable.mesh, drawable.material);
    diffuse.release();
  }
}

void GameRenderer::renderPlayer(RenderContext &rc) {
  const Entity &p = dynamic_cast<Entity &>(world.getPlayer());

  Matrix4f mtx;
  mtx.translate(p.getPosition() + Vector3f(0, -.5f, 0));
  mtx.rotate(p.getOrientation());
  mtx.scale(Vector3f(1.3f, 1.3f, 1.3f));

  const auto &dummy = MeshLoader::getMesh("HumanToken.obj");
  const auto &mat   = MaterialLoader::fromTexture("HumanToken.png");
  auto &shader      = ShaderLoader::getShader("diffuse.frag");

  renderer.renderMesh(rc, shader, mtx, dummy, mat);
}

void GameRenderer::setCameraInPortal(const Camera &cam, Camera &dest,
                                     const Entity &portal,
                                     const Entity &otherPortal) {
  Matrix4f p1mat;
  p1mat.translate(portal.getPosition());
  p1mat.rotate(portal.getOrientation());
  Matrix4f p2mat;
  p2mat.translate(otherPortal.getPosition());
  p2mat.rotate(otherPortal.getOrientation());
  Matrix4f rotate180;
  rotate180.rotate(rad(180), 0, 1, 0);
  Matrix4f view;
  cam.getViewMatrix(view);
  Matrix4f destView = view * p1mat * rotate180 * inverse(p2mat);

  dest.setPerspective();
  dest.setAspect(cam.getAspect());
  dest.setFovy(cam.getFovy());
  dest.setZNear((portal.getPosition() - cam.getPosition()).length());
  dest.setViewMatrix(destView);
}

} /* namespace glPortal */
