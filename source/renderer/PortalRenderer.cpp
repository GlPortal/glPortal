#include <glPortal/renderer/PortalRenderer.hpp>

#include <radix/data/shader/ShaderLoader.hpp>
#include <radix/renderer/Renderer.hpp>
#include <radix/core/math/Matrix3f.hpp>
#include <radix/core/gl/OpenGL.hpp>
#include <radix/data/model/MeshLoader.hpp>
#include <radix/env/Environment.hpp>

#include <glPortal/World.hpp>
#include <glPortal/renderer/GameRenderer.hpp>

namespace glPortal {
  
PortalRenderer::PortalRenderer(glPortal::World &world, radix::Renderer &renderer, radix::Camera *camera) :
  SubRenderer(world, renderer),
  camera(camera) {
}

void PortalRenderer::render() {
  
  renderContext->pushCamera(*camera);
  
  for (radix::EntityPair &pair : world.entityPairs.at("portalPairs")) {
    radix::Entity &portalEntity1 = *pair.first;
    radix::Entity &portalEntity2 = *pair.second;
    
    Portal &portal1 = dynamic_cast<Portal&>(portalEntity1);
    Portal &portal2 = dynamic_cast<Portal&>(portalEntity2);
    
    renderPortal(portal1, portal2);
    renderPortal(portal2, portal1);
    
    if (portal1.open and portal2.open) {
      radix::Shader &unshaded = radix::ShaderLoader::getShader("unshaded.frag");
      
      glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
      glDepthMask(GL_TRUE);
      glClear(GL_DEPTH_BUFFER_BIT);
      
      const radix::Mesh &portalStencil = radix::MeshLoader::getMesh("PortalStencil.obj");
      
      radix::Matrix4f matrix;
      portal1.getModelMtx(matrix);
      matrix.scale(portal1.getScaleMult());
      renderer.renderMesh(*renderContext, unshaded, matrix, portalStencil);
      
      portal2.getModelMtx(matrix);
      matrix.scale(portal2.getScaleMult());
      renderer.renderMesh(*renderContext, unshaded, matrix, portalStencil);
      
      glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    }
  }
  
  GameRenderer *gameRenderer = dynamic_cast<GameRenderer*>(renderer.getRenderer(
    typeid(GameRenderer)));
  gameRenderer->renderScene(*renderContext);
  
  glDepthMask(GL_FALSE);
  for (radix::EntityPair &pair : world.entityPairs.at("portalPairs")) {
    radix::Entity &portalEntity1 = *pair.first;
    radix::Entity &portalEntity2 = *pair.second;
    
    Portal &portal1 = dynamic_cast<Portal&>(portalEntity1);
    Portal &portal2 = dynamic_cast<Portal&>(portalEntity2);
    
    if (portal1.open and portal2.open) {
      double timeOpen = world.getTime() - std::max(portal1.openSince, portal2.openSince);
      if (timeOpen < Portal::NOISE_FADE_DELAY) {
        float adjustedTime = 1 - ((float) timeOpen / Portal::NOISE_FADE_DELAY) * 2;
        renderPortalNoise(portal1, adjustedTime);
        renderPortalNoise(portal2, adjustedTime);
      }
    } else if (portal1.open) {
      renderPortalNoise(portal1, 1.0f);
    } else if (portal2.open) {
      renderPortalNoise(portal2, 1.0f);
    }
    
    renderPortalOverlay(portal1);
    renderPortalOverlay(portal2);
  }
  glDepthMask(GL_TRUE);
  
  renderContext->popCamera();
}

void PortalRenderer::renderPortalOverlay(const Portal &portal) {
  if (portal.open) {
      radix::Matrix4f matrix;
      portal.getModelMtx(matrix);
      matrix.scale(portal.getScaleMult());
      
      renderer.renderMesh(*renderContext, radix::ShaderLoader::getShader("unshaded.frag"), matrix,
                          portal.overlayMesh, portal.overlayTex);
    }
}

void PortalRenderer::renderPortal(Portal &portal, Portal &otherPortal) {
  if (portal.open and otherPortal.open) {
    radix::Vector3f displacement = portal.getPosition() - otherPortal.getPosition();
    int recursionLevel = radix::Environment::getConfig().getRecursionLevel();
    radix::Camera portalCamera;
    
    radix::Vector3f portalPosition = portal.getPosition();
    radix::Quaternion portalOrientation = portal.getOrientation();
    radix::Vector3f portalScale = portal.getScale();
    radix::Vector3f portalDirection = portal.getDirection();
    radix::Vector3f otherPortalDirection = otherPortal.getDirection();
    float dotProduct = dot(portalDirection, otherPortalDirection);
    
    if (abs(dotProduct) < 0.0001) {
      radix::Quaternion rotationDifference = conjugate(portal.getOrientation() *
      conjugate(otherPortal.getOrientation()));
      displacement = toMatrix3f(rotationDifference.toMatrix()).transform(displacement);
      recursionLevel = 1;
      portal.setOrientation(otherPortal.getOrientation());
    } else if (dotProduct == 1.0f) {
      recursionLevel = 0;
    }
    
    GameRenderer *gameRenderer = dynamic_cast<GameRenderer*>(renderer.getRenderer(
      typeid(GameRenderer)));
    
    portal.setPosition(portal.getPosition() + displacement * (float) recursionLevel);
    for (int i = 0; i < recursionLevel; i++) {
      glEnable(GL_STENCIL_TEST);
      glClear(GL_STENCIL_BUFFER_BIT);
      glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
      glDepthMask(GL_FALSE);
      
      renderPortalStencil(portal);
      
      glStencilFunc(GL_EQUAL, 1, 0xFF);
      glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
      
      GameRenderer::setCameraInPortal(*camera, portalCamera, portal, otherPortal);
      renderContext->pushCamera(portalCamera);
      gameRenderer->renderPlayer(*renderContext);
      gameRenderer->renderScene(*renderContext);
      
      //Restore portal to original position and render portal overlay
      radix::Vector3f tempPosition = portal.getPosition();
      portal.setPosition(portalPosition);
      renderPortalOverlay(portal);
      portal.setPosition(tempPosition);
      
      renderContext->popCamera();
      
      portal.setPosition(portal.getPosition() - displacement);
      portal.setOrientation(portalOrientation);
      GameRenderer::setCameraInPortal(*camera, portalCamera, portal, otherPortal);
      glDisable(GL_STENCIL_TEST);
      
      radix::Shader &unshaded = radix::ShaderLoader::getShader("unshaded.frag");
      glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
      glDepthMask(GL_TRUE);
      glClear(GL_DEPTH_BUFFER_BIT);
      const radix::Mesh &portalStencil = radix::MeshLoader::getMesh("PortalStencil.obj");
      
      radix::Matrix4f matrix;
      matrix.translate(portalPosition);
      matrix.rotate(portalOrientation);
      matrix.scale(portalScale);
      matrix.scale(portal.getScaleMult());
      
      renderContext->pushCamera(portalCamera);
      renderer.renderMesh(*renderContext, unshaded, matrix, portalStencil);
      renderContext->popCamera();
      glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    }
    
    glEnable(GL_STENCIL_TEST);
    renderPortalStencil(portal);
    
    glStencilFunc(GL_EQUAL, 1, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
    
    GameRenderer::setCameraInPortal(*camera, portalCamera, portal, otherPortal);
    renderContext->pushCamera(portalCamera);
    gameRenderer->renderPlayer(*renderContext);
    gameRenderer->renderScene(*renderContext);
    renderPortalOverlay(portal);
    renderContext->popCamera();
    glDisable(GL_STENCIL_TEST);
  }
}

void PortalRenderer::renderPortalStencil(const Portal &portal) {
  glClear(GL_STENCIL_BUFFER_BIT);
  glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
  glDepthMask(GL_FALSE);
  glStencilFunc(GL_NEVER, 1, 0xFF);
  glStencilOp(GL_REPLACE, GL_KEEP, GL_KEEP);
  
  radix::Matrix4f matrix;
  portal.applyModelMtx(matrix);
  matrix.scale(portal.getScaleMult());
  renderer.renderMesh(*renderContext, radix::ShaderLoader::getShader("unshaded.frag"), matrix, portal.stencilMesh);
  glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
  glDepthMask(GL_TRUE);
}

void PortalRenderer::renderPortalNoise(const Portal &portal, float time) {
  radix::Matrix4f matrix;
  portal.getModelMtx(matrix);
  matrix.scale(portal.getScaleMult());
  
  radix::Shader &simplexTime = radix::ShaderLoader::getShader("simplexTime.frag");
  simplexTime.bind();
  int timeLoc = simplexTime.uni("time");
  int colorLoc = simplexTime.uni("color");
  int noiseAlphaLoc = simplexTime.uni("noiseAlpha");
  glUniform1f(timeLoc, world.getTime());
  glUniform3f(colorLoc, portal.color.x, portal.color.y, portal.color.z);
  glUniform1f(noiseAlphaLoc, time);
  
  renderer.renderMesh(*renderContext, simplexTime, matrix, portal.overlayMesh, portal.overlayTex);
  simplexTime.release();
}

} /* namespace glPortal */
