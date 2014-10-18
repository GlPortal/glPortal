#include <engine/Camera.hpp>
#include <engine/Font.hpp>
#include <engine/loader/FontLoader.hpp>
#include <engine/loader/MeshLoader.hpp>
#include <engine/loader/ShaderLoader.hpp>
#include <engine/loader/TextureLoader.hpp>
#include <engine/Letter.hpp>
#include <engine/Light.hpp>
#include <engine/Mesh.hpp>
#include <engine/renderer/Renderer.hpp>
#include <engine/Texture.hpp>
#include <Portal.hpp>
#include <stdio.h>
#include <Scene.hpp>
#include <util/math/Math.hpp>
#include <util/math/Vector2f.hpp>
#include <util/math/Vector3f.hpp>
#include <Window.hpp>
#include <vector>

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
  int numLightsLoc = glGetUniformLocation(shader.handle, "numLights");
  glUniform1i(numLightsLoc, numLights);

  //Render portals
  renderPortal(scene, scene->bluePortal, scene->orangePortal);
  renderPortal(scene, scene->orangePortal, scene->bluePortal);

  //Update camera position
  viewMatrix.setIdentity();
  viewMatrix.rotate(-scene->camera.rotation.x, 1, 0, 0);
  viewMatrix.rotate(-scene->camera.rotation.y, 0, 1, 0);
  viewMatrix.translate(Vector3f::negate(scene->camera.position));
  glUniformMatrix4fv(viewLoc, 1, false, viewMatrix.array);

  //Depth buffer
  if (scene->bluePortal.open && scene->orangePortal.open) {
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
  viewMatrix.rotate(-scene->camera.rotation.x, 1, 0, 0);
  viewMatrix.rotate(-scene->camera.rotation.y, 0, 1, 0);
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
  {
  Mesh mesh = MeshLoader::getMesh("Plane.obj");
  Texture texture = TextureLoader::getTexture("Reticle.png");
  renderTexturedMesh(mesh, texture);
  }

  //Text
  setFont("Adobe", 1.5f);
  renderText(scene, 10, Window::height - 50, "GlPortal");
}

void Renderer::renderScene(Scene* scene) {
  for (unsigned int i = 0; i < scene->walls.size(); i++) {
    renderEntity(scene->walls[i]);
  }

  for (unsigned int i = 0; i < scene->models.size(); i++) {
    renderEntity(scene->models[i]);
  }

  renderEntity(scene->end);
}

void Renderer::renderEntity(Entity e) {
  modelMatrix.setIdentity();
  modelMatrix.translate(e.position);
  modelMatrix.rotate(e.rotation);
  modelMatrix.scale(e.scale);
  glUniformMatrix4fv(modelLoc, 1, false, modelMatrix.array);
  renderTexturedMesh(e.mesh, e.texture);
}

void Renderer::renderPortal(Scene* scene, Portal portal, Portal otherPortal) {
  if (portal.open && otherPortal.open) {
    glEnable(GL_STENCIL_TEST);
    glClear(GL_STENCIL_BUFFER_BIT);
    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
    glDepthMask(GL_FALSE);

    //Update camera position
    viewMatrix.setIdentity();
    viewMatrix.rotate(-scene->camera.rotation.x, 1, 0, 0);
    viewMatrix.rotate(-scene->camera.rotation.y, 0, 1, 0);
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
    //Draw the scene from the secondary portal
    glStencilFunc(GL_EQUAL, 1, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

    setCameraInPortal(scene, portal, otherPortal);

    renderScene(scene);

    //Set the camera back to normal
    scene->camera.setPerspective();
    glUniformMatrix4fv(projLoc, 1, false, scene->camera.getProjectionMatrix().array);
    glDisable(GL_STENCIL_TEST);
  }
}

void Renderer::renderPortalOverlay(Portal portal) {
  if (portal.open) {
    modelMatrix.setIdentity();
    modelMatrix.translate(portal.position);
    modelMatrix.rotate(portal.rotation);
    modelMatrix.scale(portal.scale);
    glUniformMatrix4fv(modelLoc, 1, false, modelMatrix.array);

    renderTexturedMesh(portal.mesh, portal.texture);
  }
}

void Renderer::renderText(Scene* scene, int x, int y, std::string text) {
  glClear(GL_DEPTH_BUFFER_BIT);

  changeShader("text.frag");
  projectionMatrix = scene->camera.getProjectionMatrix();
  glUniformMatrix4fv(projLoc, 1, false, projectionMatrix.array);
  viewMatrix.setIdentity();
  glUniformMatrix4fv(viewLoc, 1, false, viewMatrix.array);
  Texture texture = TextureLoader::getTexture("Adobe.png");

  Vector2f position(x, y);
  Vector2f scaling((1.0f / Window::width), (1.0f / Window::height));

  const char* array = text.c_str();
  for (unsigned int i = 0; i < text.length(); i++) {
    char c = array[i];

    Letter letter = font.getLetter(c);
    Mesh mesh = letter.mesh;

    modelMatrix.setIdentity();
    modelMatrix.translate(position.x * scaling.x + letter.xOffset * scaling.x * font.size,
                          position.y * scaling.y + letter.yOffset * scaling.y * font.size,
                          -10);
    modelMatrix.scale(letter.width * scaling.x * font.size,
                      letter.height * scaling.y * font.size, 1);
    glUniformMatrix4fv(modelLoc, 1, false, modelMatrix.array);

    renderTexturedMesh(mesh, texture);
    position.x += letter.advance * font.size;
  }
}

void Renderer::renderTexturedMesh(Mesh mesh, Texture texture) {
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

void Renderer::setCameraInPortal(Scene* scene, Portal portal, Portal otherPortal) {
  //Set camera in other portal
  Vector3f camPos = Vector3f::sub(scene->camera.position, portal.position);
  Vector3f camDir = Math::toDirection(scene->camera.rotation);

  //Invert the position and rotation to be behind the portal
  Vector3f icamPos(-camPos.x, camPos.y, -camPos.z);
  Vector3f icamDir(-camDir.x, camDir.y, -camDir.z);

  //Calculate the position and rotation of the camera relative to the other portal
  Matrix4f mRot;
  mRot.rotate(Vector3f::negate(portal.rotation));
  mRot.rotate(otherPortal.rotation);

  Vector3f rcamPos = mRot.transform(icamPos);
  Vector3f rcamDir = mRot.transform(icamDir);

  Vector3f fcamPos = Vector3f::add(rcamPos, otherPortal.position);
  Vector3f fcamRot = Math::toEuler(rcamDir);

  //Draw only whats visible through the portal
  Camera camera;
  camera.setPerspective();
  camera.setZNear(Vector3f::sub(otherPortal.position, fcamPos).length());
  glUniformMatrix4fv(projLoc, 1, false, camera.getProjectionMatrix().array);

  viewMatrix.setIdentity();
  viewMatrix.rotate(-fcamRot.x, 1, 0, 0);
  viewMatrix.rotate(-fcamRot.y, 0, 1, 0);
  viewMatrix.translate(Vector3f::negate(fcamPos));
  glUniformMatrix4fv(viewLoc, 1, false, viewMatrix.array);
}

void Renderer::setFont(std::string font, float size) {
  this->font = FontLoader::getFont(font);
  this->font.size = size;
}

} /* namespace glPortal */
