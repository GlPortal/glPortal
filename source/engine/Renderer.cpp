#include "Renderer.hpp"

#include <stdio.h>
#include <vector>

#include "../Portal.hpp"
#include "../Scene.hpp"
#include "Camera.hpp"
#include "Light.hpp"
#include "Mesh.hpp"
#include "MeshLoader.hpp"
#include "Resource.hpp"
#include "Texture.hpp"
#include "util/Vector3f.hpp"

namespace glPortal {

Renderer::Renderer() :
    shader(Resource::loadShaders()) {
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);

  glClearColor(0, 0, 0, 1.0);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Renderer::render(Scene* scene) {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glUseProgram(shader);
  Camera camera = scene->camera;
  projectionMatrix = camera.getProjectionMatrix();

  projLoc = glGetUniformLocation(shader, "projectionMatrix");
  viewLoc = glGetUniformLocation(shader, "viewMatrix");
  modelLoc = glGetUniformLocation(shader, "modelMatrix");

  //Upload projection matrix
  glUniformMatrix4fv(projLoc, 1, false, projectionMatrix.array);


  //Lights
  int numLights = glGetUniformLocation(shader, "numLights");
  glUniform1i(numLights, scene->lights.size());

  for (unsigned int i = 0; i < scene->lights.size(); i++) {
    Light light = scene->lights[i];

    char attribute[30];
    snprintf(attribute, sizeof(attribute), "%s%d%s", "lights[", i, "].position");
    int lightPos = glGetUniformLocation(shader, attribute);
    snprintf(attribute, sizeof(attribute), "%s%d%s", "lights[", i, "].color");
    int lightColor = glGetUniformLocation(shader, attribute);
    snprintf(attribute, sizeof(attribute), "%s%d%s", "lights[", i, "].constantAtt");
    int lightConstantAtt = glGetUniformLocation(shader, attribute);
    snprintf(attribute, sizeof(attribute), "%s%d%s", "lights[", i, "].linearAtt");
    int lightLinearAtt = glGetUniformLocation(shader, attribute);
    snprintf(attribute, sizeof(attribute), "%s%d%s", "lights[", i, "].quadraticAtt");
    int lightQuadraticAtt = glGetUniformLocation(shader, attribute);

    glUniform4f(lightPos, light.position.x, light.position.y, light.position.z, 1);
    glUniform3f(lightColor, light.color.x, light.color.y, light.color.z);
    glUniform1f(lightConstantAtt, light.constantAtt);
    glUniform1f(lightLinearAtt, light.linearAtt);
    glUniform1f(lightQuadraticAtt, light.quadraticAtt);
  }

  renderPortal(scene, scene->bluePortal, scene->orangePortal);
  renderPortal(scene, scene->orangePortal, scene->bluePortal);

  //Update camera position
  viewMatrix.setIdentity();
  viewMatrix.rotate(Vector3f::negate(scene->camera.rotation));
  viewMatrix.translate(Vector3f::negate(scene->camera.position));
  glUniformMatrix4fv(viewLoc, 1, false, viewMatrix.array);

  ///
  glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
  glDepthMask(GL_TRUE);
  glClear(GL_DEPTH_BUFFER_BIT);

  modelMatrix.setIdentity();
  modelMatrix.translate(scene->bluePortal.position);
  modelMatrix.rotate(scene->bluePortal.rotation);
  modelMatrix.scale(scene->bluePortal.scale);
  glUniformMatrix4fv(modelLoc, 1, false, modelMatrix.array);

  Mesh portalStencil = MeshLoader::getMesh("data/meshes/PortalStencil.obj");
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
  ///

  renderScene(scene);

  renderPortalOverlay(scene->bluePortal);
  renderPortalOverlay(scene->orangePortal);
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

    Mesh portalStencil = MeshLoader::getMesh("data/meshes/PortalStencil.obj");
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
      viewMatrix.translate(Vector3f(-otherPortal.position.x, -otherPortal.position.y - (scene->camera.position.y - portal.position.y), -otherPortal.position.z));
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
  modelMatrix.setIdentity();
  modelMatrix.translate(portal.position);
  modelMatrix.rotate(portal.rotation);
  modelMatrix.scale(portal.scale);

  glUniformMatrix4fv(modelLoc, 1, false, modelMatrix.array);

  glBindVertexArray(portal.mesh.handle);

  int loc = glGetUniformLocation(shader, "diffuse");
  int tiling = glGetUniformLocation(shader, "tiling");
  glUniform2f(tiling, portal.texture.xTiling, portal.texture.yTiling);
  glUniform1i(loc, 0);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, portal.texture.handle);
  glDrawArrays(GL_TRIANGLES, 0, portal.mesh.numFaces * 3);
  glBindTexture(GL_TEXTURE_2D, 0);
  glBindVertexArray(0);
}

void Renderer::renderScene(Scene* scene) {
  //Walls
  for (unsigned int i = 0; i < scene->walls.size(); i++) {
    Entity wall = scene->walls[i];

    modelMatrix.setIdentity();
    modelMatrix.translate(wall.position);
    modelMatrix.rotate(wall.rotation.x, 1, 0, 0);
    modelMatrix.rotate(wall.rotation.y, 0, 1, 0);
    modelMatrix.rotate(wall.rotation.z, 0, 0, 1);
    modelMatrix.scale(wall.scale);
    glUniformMatrix4fv(modelLoc, 1, false, modelMatrix.array);

    glBindVertexArray(wall.mesh.handle);

    int loc = glGetUniformLocation(shader, "diffuse");
    int tiling = glGetUniformLocation(shader, "tiling");
    glUniform2f(tiling, wall.texture.xTiling, wall.texture.yTiling);
    glUniform1i(loc, 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, wall.texture.handle);
    glDrawArrays(GL_TRIANGLES, 0, wall.mesh.numFaces * 3);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
  }
}

} /* namespace glPortal */
