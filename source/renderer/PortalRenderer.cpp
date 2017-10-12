#include <glPortal/renderer/PortalRenderer.hpp>

#include <radix/data/shader/ShaderLoader.hpp>
#include <radix/renderer/Renderer.hpp>

#include <glPortal/World.hpp>

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
    
    renderPortalOverlay(portal1);
    renderPortalOverlay(portal2);
  }
  
  renderContext->popCamera();
}

void PortalRenderer::renderPortalOverlay(const Portal &portal) {
  if (portal.open) {
      radix::Matrix4f matrix;
      portal.getModelMtx(matrix);
      matrix.scale(portal.getScaleMult());
      
      renderer.renderMesh(*renderContext, radix::ShaderLoader::getShader("unshaded.frag"), matrix, portal.overlayMesh,
                 portal.overlayTex);
    }
}


} /* namespace glPortal */
