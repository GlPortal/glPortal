#include "Renderer.hpp"

#include <cstdio>
#include <vector>

#include <assets/model/MeshLoader.hpp>
#include <assets/texture/TextureLoader.hpp>
#include <assets/text/FontLoader.hpp>
#include <assets/shader/ShaderLoader.hpp>

#include <assets/scene/Scene.hpp>
#include <assets/model/Mesh.hpp>
#include <assets/texture/Texture.hpp>
#include <assets/text/Font.hpp>
#include <assets/text/Letter.hpp>

#include <engine/Camera.hpp>
#include <engine/Light.hpp>
#include <engine/Viewport.hpp>

#include <Window.hpp>
#include <Portal.hpp>

#include <engine/core/math/Math.hpp>
#include <engine/core/math/Vector2f.hpp>
#include <engine/core/math/Vector3f.hpp>

#include <SDL2/SDL_timer.h>

namespace glPortal {

Renderer::Renderer() : viewport(nullptr) {
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);

  glClearColor(0, 0, 0, 1.0);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Renderer::setViewport(Viewport *vp) {
  viewport = vp;
}

/**
 * Sets the current scene for rendering
 */
void Renderer::setScene(Scene *scene) {
  this->scene = scene;
}

/**
 * Changes the current shader to the shader located at path
 * @param path The path of the shader
 */
void Renderer::changeShader(const std::string &path) {
  this->shader = &ShaderLoader::getShader(path);
  glUseProgram(shader->handle);

  projLoc = glGetUniformLocation(shader->handle, "projectionMatrix");
  viewLoc = glGetUniformLocation(shader->handle, "viewMatrix");
  modelLoc = glGetUniformLocation(shader->handle, "modelMatrix");
  normalLoc = glGetUniformLocation(shader->handle, "normalMatrix");
}

/**
 * Sets the font to use for all future text drawing until changed again
 * @param font Name of the font
 * @param size Size of the text drawn with this font
 */
void Renderer::setFont(const std::string &font, float size) {
  this->font = &FontLoader::getFont(font);
  this->font->size = size;
}


void Renderer::render() {
  viewport->getSize(&vpWidth, &vpHeight);

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  Camera &camera = scene->camera;
  camera.setPerspective();
  camera.setAspect((float)vpWidth / vpHeight);
  camera.loadMatrix(projectionMatrix);

  changeShader("diffuse.frag");

  //Lights
  for (unsigned int i = 0; i < scene->lights.size(); i++) {
    const Light &light = scene->lights[i];

    char attribute[30];
    snprintf(attribute, sizeof(attribute), "%s%d%s", "lights[", i, "].position");
    int lightPos = glGetUniformLocation(shader->handle, attribute);
    snprintf(attribute, sizeof(attribute), "%s%d%s", "lights[", i, "].color");
    int lightColor = glGetUniformLocation(shader->handle, attribute);
    snprintf(attribute, sizeof(attribute), "%s%d%s", "lights[", i, "].distance");
    int lightDistance = glGetUniformLocation(shader->handle, attribute);
    snprintf(attribute, sizeof(attribute), "%s%d%s", "lights[", i, "].energy");
    int lightEnergy = glGetUniformLocation(shader->handle, attribute);

    glUniform3f(lightPos, light.position.x, light.position.y, light.position.z);
    glUniform3f(lightColor, light.color.x, light.color.y, light.color.z);
    glUniform1f(lightDistance, light.distance);
    glUniform1f(lightEnergy, light.energy);
  }

  int numLights = scene->lights.size();
  int numLightsLoc = glGetUniformLocation(shader->handle, "numLights");
  glUniform1i(numLightsLoc, numLights);

  //Render portals
  renderPortal(scene->bluePortal, scene->orangePortal);
  renderPortal(scene->orangePortal, scene->bluePortal);

  //Update camera position
  setCameraInPlayer(scene->camera);

  // Depth buffer
  if (scene->bluePortal.open and scene->orangePortal.open) {
    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
    glDepthMask(GL_TRUE);
    glClear(GL_DEPTH_BUFFER_BIT);

    modelMatrix.setIdentity();
    modelMatrix.translate(scene->bluePortal.position);
    modelMatrix.rotate(scene->bluePortal.rotation);
    modelMatrix.scale(scene->bluePortal.scale);
    glUniformMatrix4fv(modelLoc, 1, false, modelMatrix.array);

    const Mesh &portalStencil = MeshLoader::getMesh("PortalStencil.obj");
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

  renderScene();

  // Draw simplex noise
  changeShader("simplexTime.frag");
  
  setCameraInPlayer(scene->camera);

  glDepthMask(GL_FALSE);
  if (scene->orangePortal.open and scene->bluePortal.open) {
    uint32_t dtOpen = SDL_GetTicks()-max(scene->orangePortal.openSince, scene->bluePortal.openSince);
    if (dtOpen < Portal::NOISE_FADE_DELAY) {
      float al = 1-((float)dtOpen/Portal::NOISE_FADE_DELAY)*2;
      printf("%d\n", dtOpen);
      if (true or not scene->orangePortal.open) {
        renderPortalNoise(scene->bluePortal, al);
      }
      if (true or not scene->bluePortal.open) {
        renderPortalNoise(scene->orangePortal, al);
      }
    }
  } else {
    if (scene->orangePortal.open) {
      renderPortalNoise(scene->orangePortal, 1.f);
    }
    if (scene->bluePortal.open) {
      renderPortalNoise(scene->bluePortal, 1.f);
    }
  }
  glDepthMask(GL_TRUE);

  // Draw overlays
  changeShader("unshaded.frag");

  // Update camera position
  setCameraInPlayer(scene->camera);

  renderPortalOverlay(scene->bluePortal);
  renderPortalOverlay(scene->orangePortal);

  //Draw GUI
  glClear(GL_DEPTH_BUFFER_BIT);
  camera.setOrthographic();
  camera.setBounds(0, vpWidth, 0, vpHeight);

  //Upload matrices
  camera.loadMatrix(projectionMatrix);
  glUniformMatrix4fv(projLoc, 1, false, projectionMatrix.array);
  viewMatrix.setIdentity();
  glUniformMatrix4fv(viewLoc, 1, false, viewMatrix.array);

  //Crosshair
  {
  modelMatrix.setIdentity();
  modelMatrix.translate(Vector3f(vpWidth/2, vpHeight/2, -2));
  modelMatrix.scale(Vector3f(80, 80, 1));
  glUniformMatrix4fv(modelLoc, 1, false, modelMatrix.array);
  const Mesh &mesh = MeshLoader::getMesh("GUIElement.obj");
  Texture texture = TextureLoader::getTexture("Reticle.png");
  renderTexturedMesh(mesh, texture);
  }

  //Text
  setFont("Adobe", 1.5f);
  renderText("GlPortal", 25, vpHeight - 75);
}

void Renderer::renderScene() {
  for (unsigned int i = 0; i < scene->walls.size(); i++) {
    renderEntity(scene->walls[i]);
  }

  for (unsigned int i = 0; i < scene->models.size(); i++) {
    renderEntity(scene->models[i]);
  }

  renderEntity(scene->end);
}

void Renderer::renderEntity(const VisualEntity &e) {
  modelMatrix.setIdentity();
  modelMatrix.translate(e.position);
  modelMatrix.rotate(e.rotation);
  modelMatrix.scale(e.scale);
  glUniformMatrix4fv(modelLoc, 1, false, modelMatrix.array);
  Matrix4f normalMatrix = inverse(modelMatrix);
  glUniformMatrix4fv(normalLoc, 1, false, normalMatrix.array);

  renderTexturedMesh(e.mesh, e.texture);
}

void Renderer::renderPortal(const Portal &portal, const Portal &otherPortal) {
  if (portal.open and otherPortal.open) {
    glEnable(GL_STENCIL_TEST);
    glClear(GL_STENCIL_BUFFER_BIT);
    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
    glDepthMask(GL_FALSE);

    //Update camera position
    setCameraInPlayer(scene->camera);

    //Stencil drawing
    //Primary portal
    glStencilFunc(GL_NEVER, 1, 0xFF);
    glStencilOp(GL_REPLACE, GL_KEEP, GL_KEEP);

    modelMatrix.setIdentity();
    modelMatrix.translate(portal.position);
    modelMatrix.rotate(portal.rotation);
    modelMatrix.scale(portal.scale);
    glUniformMatrix4fv(modelLoc, 1, false, modelMatrix.array);

    const Mesh &portalStencil = MeshLoader::getMesh("PortalStencil.obj");
    glBindVertexArray(portalStencil.handle);
    glDrawArrays(GL_TRIANGLES, 0, portalStencil.numFaces * 3);
    glBindVertexArray(0);

    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    glDepthMask(GL_TRUE);
    //Draw the scene from the secondary portal
    glStencilFunc(GL_EQUAL, 1, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

    setCameraInPortal(portal, otherPortal);

    renderScene();

    //Set the camera back to normal
    scene->camera.setPerspective();
    scene->camera.setAspect((float)vpWidth / vpHeight);
    scene->camera.loadMatrix(projectionMatrix);
    glUniformMatrix4fv(projLoc, 1, false, projectionMatrix.array);
    glDisable(GL_STENCIL_TEST);
  }
}

void Renderer::renderPortalOverlay(const Portal& portal) {
  if (portal.open) {
    modelMatrix.setIdentity();
    modelMatrix.translate(portal.position);
    modelMatrix.rotate(portal.rotation);
    modelMatrix.scale(portal.scale);
    glUniformMatrix4fv(modelLoc, 1, false, modelMatrix.array);

    renderTexturedMesh(portal.mesh, portal.texture);
  }
}

void Renderer::renderPortalNoise(const Portal &portal, float fade) {
  modelMatrix.setIdentity();
  modelMatrix.translate(portal.position);
  modelMatrix.rotate(portal.rotation);
  modelMatrix.scale(portal.scale);
  glUniformMatrix4fv(modelLoc, 1, false, modelMatrix.array);

  int timeLoc = glGetUniformLocation(shader->handle, "time");
  int colorLoc = glGetUniformLocation(shader->handle, "color");
  int noiseAlphaLoc = glGetUniformLocation(shader->handle, "noiseAlpha");
  glUniform1f(timeLoc, SDL_GetTicks()/1000.f);
  glUniform3f(colorLoc, portal.color.x, portal.color.y, portal.color.z);
  glUniform1f(noiseAlphaLoc, fade);

  renderTexturedMesh(portal.mesh, portal.maskTex);
}

void Renderer::renderText(const std::string &text, int x, int y) {
  glClear(GL_DEPTH_BUFFER_BIT);

  changeShader("text.frag");
  scene->camera.loadMatrix(projectionMatrix);
  glUniformMatrix4fv(projLoc, 1, false, projectionMatrix.array);
  viewMatrix.setIdentity();
  glUniformMatrix4fv(viewLoc, 1, false, viewMatrix.array);
  Texture texture = TextureLoader::getTexture("Adobe.png");

  Vector2f position(x, y);

  const char *array = text.c_str();
  for (unsigned int i = 0; i < text.length(); i++) {
    char c = array[i];

    const Letter &letter = font->getLetter(c);
    const Mesh &mesh = letter.mesh;

    modelMatrix.setIdentity();
    modelMatrix.translate(position.x + letter.xOffset * font->size,
                          position.y + letter.yOffset * font->size,
                          -10);

    modelMatrix.scale(letter.width * font->size,
                      letter.height * font->size, 1);
    glUniformMatrix4fv(modelLoc, 1, false, modelMatrix.array);

    renderTexturedMesh(mesh, texture);
    position.x += letter.advance * font->size;
  }
}

/**
 * Renders a mesh with the specified texture
 * @param mesh The mesh to render
 */
void Renderer::renderTexturedMesh(const Mesh &mesh, const Texture &texture) {
  int loc = glGetUniformLocation(shader->handle, "diffuse");
  int tiling = glGetUniformLocation(shader->handle, "tiling");
  glUniform2f(tiling, texture.xTiling, texture.yTiling);
  glUniform1i(loc, 0);
  glActiveTexture(GL_TEXTURE0);

  glBindVertexArray(mesh.handle);
  glBindTexture(GL_TEXTURE_2D, texture.handle);
  glDrawArrays(GL_TRIANGLES, 0, mesh.numFaces * 3);
  glBindTexture(GL_TEXTURE_2D, 0);
  glBindVertexArray(0);
}

void Renderer::setCameraInPortal(const Portal &portal, const Portal &otherPortal) {
  //Set camera in other portal
  Vector3f camPos = scene->camera.position - portal.position;
  Vector3f camDir = Math::toDirection(scene->camera.rotation);

  //Invert the position and rotation to be behind the portal
  Vector3f icamPos(-camPos.x, camPos.y, -camPos.z);
  Vector3f icamDir(-camDir.x, camDir.y, -camDir.z);

  //Calculate the position and rotation of the camera relative to the other portal
  Matrix4f mRot;
  mRot.rotate(negate(portal.rotation));
  mRot.rotate(otherPortal.rotation);

  Vector3f rcamPos = mRot.transform(icamPos);
  Vector3f rcamDir = mRot.transform(icamDir);

  Vector3f fcamPos = rcamPos + otherPortal.position;
  Vector3f fcamRot = Math::toEuler(rcamDir);

  //Draw only whats visible through the portal
  Camera camera;
  camera.setPerspective();
  camera.setAspect((float)vpWidth / vpHeight);
  camera.setZNear((otherPortal.position - fcamPos).length());
  camera.loadMatrix(projectionMatrix);
  glUniformMatrix4fv(projLoc, 1, false, projectionMatrix.array);

  viewMatrix.setIdentity();
  viewMatrix.rotate(-fcamRot.x, 1, 0, 0);
  viewMatrix.rotate(-fcamRot.y, 0, 1, 0);
  viewMatrix.translate(negate(fcamPos));
  glUniformMatrix4fv(viewLoc, 1, false, viewMatrix.array);
}

void Renderer::setCameraInPlayer(const Camera& camera) {
  camera.loadMatrix(projectionMatrix);
  glUniformMatrix4fv(projLoc, 1, false, projectionMatrix.array);

  viewMatrix.setIdentity();
  viewMatrix.rotate(-camera.rotation.x, 1, 0, 0);
  viewMatrix.rotate(-camera.rotation.y, 0, 1, 0);
  viewMatrix.translate(negate(camera.position));
  glUniformMatrix4fv(viewLoc, 1, false, viewMatrix.array);
}

} /* namespace glPortal */
