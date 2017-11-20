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

namespace glPortal {

GameRenderer::GameRenderer(glPortal::World &w, radix::Renderer &ren,
                           radix::Camera *cam, double *ptime)
    : SubRenderer(dynamic_cast<radix::World&>(w), ren), camera(cam), dtime(ptime) {}

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
  if (renderContext.viewFramesStack.size() > 0) {
    const RenderContext::ViewFrameInfo &vfi = renderContext.getViewFrame();
    // Don't render further if computed clip rect is zero-sized
    if (not renderer.clipViewFrame(renderContext, vfi.first, vfi.second,
                                   scissor)) {
      return;
    }
  }

  glClear(GL_DEPTH_BUFFER_BIT);

  renderViewFrames(renderContext);

  if (renderContext.viewFramesStack.size() > 0) {
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
  if (dbgDraw) {
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
    if (vf) {
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
  if (not save_stencil_test) {
    glDisable(GL_STENCIL_TEST);
  }
  if (not save_scissor_test) {
    glDisable(GL_SCISSOR_TEST);
  }

  // Draw portal in the depth buffer so they are not overwritten
  glClear(GL_DEPTH_BUFFER_BIT);

  std::array<GLboolean, 4> save_color_mask {};
  GLboolean save_depth_mask;
  glGetBooleanv(GL_COLOR_WRITEMASK, save_color_mask.data());
  glGetBooleanv(GL_DEPTH_WRITEMASK, &save_depth_mask);
  glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
  glDepthMask(GL_TRUE);
  Shader shader = ShaderLoader::getShader("whitefill.frag");
  Matrix4f modelMtx;
  for(auto& view : rc.viewFramesStack) {
    renderer.renderMesh(rc, shader, modelMtx, view.first, nullptr);
  }
  shader.release();
  glColorMask(save_color_mask[0], save_color_mask[1], save_color_mask[2],
              save_color_mask[3]);
  glDepthMask(save_depth_mask);
}

void GameRenderer::renderViewFrameStencil(RenderContext &rc) {
  PROFILER_BLOCK("GameRenderer::renderViewFrameStencil");
  std::array<GLboolean, 4> save_color_mask {};
  GLboolean save_depth_mask;
  glGetBooleanv(GL_COLOR_WRITEMASK, save_color_mask.data());
  glGetBooleanv(GL_DEPTH_WRITEMASK, &save_depth_mask);

  glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
  glDepthMask(GL_FALSE);
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

  glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
  glDepthMask(GL_TRUE);
  glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
  /* Fill 1 or more */
  glStencilFunc(GL_LEQUAL, 1, 0xFF);
  glColorMask(save_color_mask[0], save_color_mask[1], save_color_mask[2],
              save_color_mask[3]);
  glDepthMask(save_depth_mask);
}

void GameRenderer::renderEntities(RenderContext &rc) {
  for (Entity &e : world.entityManager) {
    if (dynamic_cast<const entities::MeshDrawableTrait *>(&e) != nullptr) {
      renderEntity(rc, e);
    }
  }
}

void GameRenderer::renderPortals(RenderContext &rc) {
  auto index = world.entityPairs.find("portalPairs");
  if(index == world.entityPairs.end() || index->second.empty()) {
    return;
  }

  auto& portals = world.entityPairs["portalPairs"];

  renderPortal(portals.back().first, rc, renderer);
  renderPortal(portals.back().second, rc, renderer);
}

void  GameRenderer::renderPortal(Entity* portal, RenderContext& rc, Renderer& renderer) {
  auto p = dynamic_cast<Portal*>(portal);
  if(p == nullptr) return;

  auto &mesh   = p->overlayMesh;
  auto &mat    = p->overlayTex;

  auto &mesh2  = p->stencilMesh;
  auto &mat2   = p->maskTex;

  Matrix4f mtx;
  mtx.translate(portal->getPosition());
  mtx.rotate(portal->getOrientation());
  mtx.scale(Vector3f(1.f, 2.f, 1.f));

  auto &shader2 = ShaderLoader::getShader("color.frag");
  auto &shader  = ShaderLoader::getShader("unshaded.frag");

  if(mesh2.numFaces != 0) renderer.renderMesh(rc, shader2, mtx, mesh2, mat2);
  if(mesh.numFaces  != 0) renderer.renderMesh(rc, shader,  mtx, mesh,  mat);

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
  const Entity &p = dynamic_cast<Entity&>(world.getPlayer());

  Matrix4f mtx;
  mtx.translate(p.getPosition() + Vector3f(0, -.5f, 0));
  mtx.rotate(p.getOrientation());
  mtx.scale(Vector3f(1.3f, 1.3f, 1.3f));

  const Mesh &dummy   = MeshLoader::getMesh("HumanToken.obj");
  const Material &mat = MaterialLoader::fromTexture("HumanToken.png");

  renderer.renderMesh(rc, ShaderLoader::getShader("diffuse.frag"), mtx, dummy,
                      mat);
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
