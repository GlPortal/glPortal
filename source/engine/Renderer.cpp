#include "Renderer.hpp"

#include <stdio.h>
#include <vector>

#include "../Portal.hpp"
#include "../Scene.hpp"
#include "Camera.hpp"
#include "Light.hpp"
#include "Mesh.hpp"
#include "MeshLoader.hpp"
#include "ShaderLoader.hpp"
#include "Texture.hpp"
#include "TextureLoader.hpp"
#include "util/Vector3f.hpp"

namespace glPortal {

Renderer::Renderer() {
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);

  glClearColor(0, 0, 0, 1.0);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Renderer::changeShader(std::string path) {
  this->shader = ShaderLoader::getShader(path);
  glUseProgram(shader.handle);

  projLoc = glGetUniformLocation(shader.handle, "projectionMatrix");
  viewLoc = glGetUniformLocation(shader.handle, "viewMatrix");
  modelLoc = glGetUniformLocation(shader.handle, "modelMatrix");
}

void Renderer::render(Scene* scene) {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  Camera camera = scene->camera;
  camera.setPerspective();
  projectionMatrix = camera.getProjectionMatrix();

  changeShader("diffuse.frag");

  //Upload projection matrix
  glUniformMatrix4fv(projLoc, 1, false, projectionMatrix.array);

  //Lights
  for (unsigned int i = 0; i < scene->lights.size(); i++) {
    Light light = scene->lights[i];

    char attribute[30];
    snprintf(attribute, sizeof(attribute), "%s%d%s", "lights[", i, "].position");
    int lightPos = glGetUniformLocation(shader.handle, attribute);
    snprintf(attribute, sizeof(attribute), "%s%d%s", "lights[", i, "].color");
    int lightColor = glGetUniformLocation(shader.handle, attribute);
    snprintf(attribute, sizeof(attribute), "%s%d%s", "lights[", i, "].constantAtt");
    int lightConstantAtt = glGetUniformLocation(shader.handle, attribute);
    snprintf(attribute, sizeof(attribute), "%s%d%s", "lights[", i, "].linearAtt");
    int lightLinearAtt = glGetUniformLocation(shader.handle, attribute);
    snprintf(attribute, sizeof(attribute), "%s%d%s", "lights[", i, "].quadraticAtt");
    int lightQuadraticAtt = glGetUniformLocation(shader.handle, attribute);

    glUniform4f(lightPos, light.position.x, light.position.y, light.position.z, 1);
    glUniform3f(lightColor, light.color.x, light.color.y, light.color.z);
    glUniform1f(lightConstantAtt, light.constantAtt);
    glUniform1f(lightLinearAtt, light.linearAtt);
    glUniform1f(lightQuadraticAtt, light.quadraticAtt);
  }

  int numLights = scene->lights.size();
  if(scene->bluePortal.open) {
    Light light = scene->bluePortal.light;
    char attribute[30];
    snprintf(attribute, sizeof(attribute), "%s%d%s", "lights[", numLights, "].position");
    int lightPos = glGetUniformLocation(shader.handle, attribute);
    snprintf(attribute, sizeof(attribute), "%s%d%s", "lights[", numLights, "].color");
    int lightColor = glGetUniformLocation(shader.handle, attribute);
    snprintf(attribute, sizeof(attribute), "%s%d%s", "lights[", numLights, "].constantAtt");
    int lightConstantAtt = glGetUniformLocation(shader.handle, attribute);
    snprintf(attribute, sizeof(attribute), "%s%d%s", "lights[", numLights, "].linearAtt");
    int lightLinearAtt = glGetUniformLocation(shader.handle, attribute);
    snprintf(attribute, sizeof(attribute), "%s%d%s", "lights[", numLights, "].quadraticAtt");
    int lightQuadraticAtt = glGetUniformLocation(shader.handle, attribute);

    glUniform4f(lightPos, light.position.x, light.position.y, light.position.z, 1);
    glUniform3f(lightColor, light.color.x, light.color.y, light.color.z);
    glUniform1f(lightConstantAtt, light.constantAtt);
    glUniform1f(lightLinearAtt, light.linearAtt);
    glUniform1f(lightQuadraticAtt, light.quadraticAtt);
    numLights++;
  }
  if(scene->orangePortal.open) {
    Light light = scene->orangePortal.light;
    char attribute[30];
    snprintf(attribute, sizeof(attribute), "%s%d%s", "lights[", numLights, "].position");
    int lightPos = glGetUniformLocation(shader.handle, attribute);
    snprintf(attribute, sizeof(attribute), "%s%d%s", "lights[", numLights, "].color");
    int lightColor = glGetUniformLocation(shader.handle, attribute);
    snprintf(attribute, sizeof(attribute), "%s%d%s", "lights[", numLights, "].constantAtt");
    int lightConstantAtt = glGetUniformLocation(shader.handle, attribute);
    snprintf(attribute, sizeof(attribute), "%s%d%s", "lights[", numLights, "].linearAtt");
    int lightLinearAtt = glGetUniformLocation(shader.handle, attribute);
    snprintf(attribute, sizeof(attribute), "%s%d%s", "lights[", numLights, "].quadraticAtt");
    int lightQuadraticAtt = glGetUniformLocation(shader.handle, attribute);

    glUniform4f(lightPos, light.position.x, light.position.y, light.position.z, 1);
    glUniform3f(lightColor, light.color.x, light.color.y, light.color.z);
    glUniform1f(lightConstantAtt, light.constantAtt);
    glUniform1f(lightLinearAtt, light.linearAtt);
    glUniform1f(lightQuadraticAtt, light.quadraticAtt);
    numLights++;
  }
  int numLightsLoc = glGetUniformLocation(shader.handle, "numLights");
  glUniform1i(numLightsLoc, numLights);

  //Render portals
  renderPortal(scene, scene->bluePortal, scene->orangePortal);
  renderPortal(scene, scene->orangePortal, scene->bluePortal);

  //Update camera position
  viewMatrix.setIdentity();
  viewMatrix.rotate(Vector3f::negate(scene->camera.rotation));
  viewMatrix.translate(Vector3f::negate(scene->camera.position));
  glUniformMatrix4fv(viewLoc, 1, false, viewMatrix.array);

  //Depth buffer
  if(scene->bluePortal.open && scene->orangePortal.open) {
    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
    glDepthMask(GL_TRUE);
    glClear(GL_DEPTH_BUFFER_BIT);

    modelMatrix.setIdentity();
    modelMatrix.translate(scene->bluePortal.position);
    modelMatrix.rotate(scene->bluePortal.rotation);
    modelMatrix.scale(scene->bluePortal.scale);
    glUniformMatrix4fv(modelLoc, 1, false, modelMatrix.array);

    Mesh portalStencil = MeshLoader::getMesh("PortalStencil.obj");
    glBindVertexArray(portalStencil.handle);
    glDrawArrays(GL_TRIANGLES, 0, portalStencil.numFaces * 3);

    modelMatrix.setIdentity();
    modelMatrix.translate(scene->orangePortal.position);
    modelMatrix.rotate(scene->orangePortal.rotation);
    modelMatrix.scale(scene->orangePortal.scale);
    glUniformMatrix4fv(modelLoc, 1, false, modelMatrix.array);

    glDrawArrays(GL_TRIANGLES, 0, portalStencil.numFaces * 3);
    glBindVertexArray(0);

    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
  }

  renderScene(scene);

  //Draw overlays
  changeShader("unshaded.frag");

  projectionMatrix = camera.getProjectionMatrix();
  glUniformMatrix4fv(projLoc, 1, false, projectionMatrix.array);

  //Update camera position
  viewMatrix.setIdentity();
  viewMatrix.rotate(Vector3f::negate(scene->camera.rotation));
  viewMatrix.translate(Vector3f::negate(scene->camera.position));
  glUniformMatrix4fv(viewLoc, 1, false, viewMatrix.array);

  renderPortalOverlay(scene->bluePortal);
  renderPortalOverlay(scene->orangePortal);

  //Draw GUI
  glClear(GL_DEPTH_BUFFER_BIT);
  scene->camera.setOrthographic();
  //Upload matrices
  projectionMatrix = camera.getProjectionMatrix();
  glUniformMatrix4fv(projLoc, 1, false, projectionMatrix.array);
  viewMatrix.setIdentity();
  glUniformMatrix4fv(viewLoc, 1, false, viewMatrix.array);
  modelMatrix.setIdentity();
  modelMatrix.translate(Vector3f(0, 0, -1));
  modelMatrix.rotate(180, 0, 1, 0);
  modelMatrix.scale(Vector3f(0.1f, 0.1f, 1));
  glUniformMatrix4fv(modelLoc, 1, false, modelMatrix.array);

  //Crosshair
  Mesh mesh = MeshLoader::getMesh("Plane.obj");
  Texture texture = TextureLoader::getTexture("Reticle.png");
  glBindVertexArray(mesh.handle);

  int loc = glGetUniformLocation(shader.handle, "diffuse");
  int tiling = glGetUniformLocation(shader.handle, "tiling");
  glUniform2f(tiling, texture.xTiling, texture.yTiling);
  glUniform1i(loc, 0);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture.handle);
  glDrawArrays(GL_TRIANGLES, 0, mesh.numFaces * 3);
  glBindTexture(GL_TEXTURE_2D, 0);
  glBindVertexArray(0);
}

void Renderer::renderScene(Scene* scene) {
  //Walls
  for (unsigned int i = 0; i < scene->walls.size(); i++) {
    renderEntity(scene->walls[i]);
  }
  //End
  renderEntity(scene->end);
}

void Renderer::renderEntity(Entity e) {
  modelMatrix.setIdentity();
  modelMatrix.translate(e.position);
  modelMatrix.rotate(e.rotation);
  modelMatrix.scale(e.scale);
  glUniformMatrix4fv(modelLoc, 1, false, modelMatrix.array);

  glBindVertexArray(e.mesh.handle);

  int loc = glGetUniformLocation(shader.handle, "diffuse");
  int tiling = glGetUniformLocation(shader.handle, "tiling");
  glUniform2f(tiling, e.texture.xTiling, e.texture.yTiling);
  glUniform1i(loc, 0);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, e.texture.handle);
  glDrawArrays(GL_TRIANGLES, 0, e.mesh.numFaces * 3);
  glBindTexture(GL_TEXTURE_2D, 0);
  glBindVertexArray(0);
}

void Renderer::renderPortal(Scene* scene, Portal portal, Portal otherPortal) {
  if (portal.open && otherPortal.open) {
    glEnable(GL_STENCIL_TEST);
    glClear(GL_STENCIL_BUFFER_BIT);
    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
    glDepthMask(GL_FALSE);

    //Update camera position
    viewMatrix.setIdentity();
    viewMatrix.rotate(Vector3f::negate(scene->camera.rotation));
    viewMatrix.translate(Vector3f::negate(scene->camera.position));
    glUniformMatrix4fv(viewLoc, 1, false, viewMatrix.array);

    //Stencil drawing
    //Primary portal
    glStencilFunc(GL_NEVER, 1, 0xFF);
    glStencilOp(GL_REPLACE, GL_KEEP, GL_KEEP);

    modelMatrix.setIdentity();
    modelMatrix.translate(portal.position);
    modelMatrix.rotate(portal.rotation);
    modelMatrix.scale(portal.scale);
    glUniformMatrix4fv(modelLoc, 1, false, modelMatrix.array);

    Mesh portalStencil = MeshLoader::getMesh("PortalStencil.obj");
    glBindVertexArray(portalStencil.handle);
    glDrawArrays(GL_TRIANGLES, 0, portalStencil.numFaces * 3);
    glBindVertexArray(0);

    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    glDepthMask(GL_TRUE);
    //Draw the scene from the other portal
    glStencilFunc(GL_EQUAL, 1, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

    //Set camera in other portal
    if(portal.rotation.x == 0) {
      viewMatrix.setIdentity();
      viewMatrix.rotate(-otherPortal.rotation.x - scene->camera.rotation.x, 1, 0, 0);
      viewMatrix.rotate(-otherPortal.rotation.y - (scene->camera.rotation.y + 180 - portal.rotation.y), 0, 1, 0);
      viewMatrix.rotate(-otherPortal.rotation.z, 0, 0, 1);
      viewMatrix.translate(
      Vector3f(-otherPortal.position.x,
               -otherPortal.position.y - (scene->camera.position.y - portal.position.y),
               -otherPortal.position.z));
    } else {
      viewMatrix.setIdentity();
      viewMatrix.rotate(-otherPortal.rotation.x, 1, 0, 0);
      viewMatrix.rotate(-otherPortal.rotation.y, 0, 1, 0);
      viewMatrix.rotate(-otherPortal.rotation.z, 0, 0, 1);
      viewMatrix.translate(Vector3f::negate(otherPortal.position));
    }
    glUniformMatrix4fv(viewLoc, 1, false, viewMatrix.array);

    renderScene(scene);

    glDisable(GL_STENCIL_TEST);
  }
}

void Renderer::renderPortalOverlay(Portal portal) {
  if(portal.open) {
    modelMatrix.setIdentity();
    modelMatrix.translate(portal.position);
    modelMatrix.rotate(portal.rotation);
    modelMatrix.scale(portal.scale);

    glUniformMatrix4fv(modelLoc, 1, false, modelMatrix.array);

    glBindVertexArray(portal.mesh.handle);

    int loc = glGetUniformLocation(shader.handle, "diffuse");
    int tiling = glGetUniformLocation(shader.handle, "tiling");
    glUniform2f(tiling, portal.texture.xTiling, portal.texture.yTiling);
    glUniform1i(loc, 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, portal.texture.handle);
    glDrawArrays(GL_TRIANGLES, 0, portal.mesh.numFaces * 3);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
  }
}

} /* namespace glPortal */
