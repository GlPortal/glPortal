#include "Renderer.hpp"

#include <cstdio>
#include <vector>
#include <string>

#include <epoxy/gl.h>

#include <assets/model/MeshLoader.hpp>
#include <assets/texture/TextureLoader.hpp>
#include <assets/text/FontLoader.hpp>
#include <assets/shader/ShaderLoader.hpp>

#include <assets/scene/Scene.hpp>
#include <assets/model/Mesh.hpp>
#include <assets/texture/Texture.hpp>
#include <assets/text/Font.hpp>
#include <assets/text/Letter.hpp>
#include <assets/material/MaterialLoader.hpp>

#include <engine/Camera.hpp>
#include <engine/Viewport.hpp>

#include <Window.hpp>
#include <Portal.hpp>

#include <engine/core/math/Math.hpp>
#include <engine/core/math/Matrix3f.hpp>
#include <engine/core/math/Vector2f.hpp>
#include <engine/core/math/Vector3f.hpp>
#include <engine/core/math/Vector4f.hpp>

#include <engine/component/Transform.hpp>
#include <engine/component/MeshDrawable.hpp>
#include <engine/component/LightSource.hpp>
#include <Game.hpp>

#include <SDL2/SDL_timer.h>
#include <algorithm>
#include "UiRenderer.hpp"

namespace glPortal {

  Renderer::Renderer() : shader(nullptr), font(nullptr), vpWidth(0), vpHeight(0),
                         scene(nullptr), viewport(nullptr),
                         portalDepth(2), fontColor(1, 1, 1, 1){
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

Viewport* Renderer::getViewport() const {
  return viewport;
}

void Renderer::setScene(Scene *scene) {
  this->scene = scene;
}

void Renderer::setShader(Shader *shader) {
  this->shader = shader;
  glUseProgram(shader->handle);
}

Camera Renderer::getCamera() {
  return scene->camera;
}


void Renderer::setFont(const std::string &font, float size) {
  this->font = &FontLoader::getFont(font);
  this->font->size = size;
}

void Renderer::setFontSize(float size) {
  this->font->size = size;
}

void Renderer::setFontColor(const Vector4f color) {
  this->fontColor = color;
}

int Renderer::getTextWidth(std::string text) {
  return this->font->getStringLength(text);
}

void Renderer::render() {
  viewport->getSize(&vpWidth, &vpHeight);

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  Camera &camera = scene->camera;
  camera.setPerspective();
  camera.setAspect((float)vpWidth / vpHeight);

  /* Portals, pass 1 */
  for (EntityPair &p : scene->portalPairs) {
    Entity &pEnt1 = *p.first,
           &pEnt2 = *p.second;
    Portal &portal1 = pEnt1.getComponent<Portal>(),
           &portal2 = pEnt2.getComponent<Portal>();

    // Render portals
    renderPortal(camera, pEnt1, pEnt2);
    renderPortal(camera, pEnt2, pEnt1);
    // Depth buffer
    if (portal1.open and portal2.open) {
      setShader(&ShaderLoader::getShader("unshaded.frag"));

      glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
      glDepthMask(GL_TRUE);
      glClear(GL_DEPTH_BUFFER_BIT);

      const Mesh &portalStencil = MeshLoader::getMesh("PortalStencil.obj");

      Matrix4f mtx;
      pEnt1.getComponent<Transform>().getModelMtx(mtx);
      mtx.scale(portal1.getScaleMult());
      renderMesh(camera, mtx, portalStencil);

      pEnt2.getComponent<Transform>().getModelMtx(mtx);
      mtx.scale(portal2.getScaleMult());
      renderMesh(camera, mtx, portalStencil);

      glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    }
  }

  renderScene(camera);

  /* Portals, pass 2 */
  glDepthMask(GL_FALSE);
  for (EntityPair &p : scene->portalPairs) {
    Entity &pEnt1 = *p.first,
           &pEnt2 = *p.second;
    Portal &portal1 = pEnt1.getComponent<Portal>(),
           &portal2 = pEnt2.getComponent<Portal>();

    // Draw simplex noise
    if (portal1.open and portal2.open) {
      uint32_t dtOpen = SDL_GetTicks()-std::max(portal1.openSince, portal2.openSince);
      if (dtOpen < Portal::NOISE_FADE_DELAY) {
        float al = 1-((float)dtOpen/Portal::NOISE_FADE_DELAY)*2;
        renderPortalNoise(camera, pEnt1, al);
        renderPortalNoise(camera, pEnt2, al);
      }
    } else {
      if (portal1.open) {
        renderPortalNoise(camera, pEnt1, 1.f);
      }
      if (portal2.open) {
        renderPortalNoise(camera, pEnt2, 1.f);
      }
    }

    // Draw overlays
    renderPortalOverlay(camera, pEnt1);
    renderPortalOverlay(camera, pEnt2);
  }
  glDepthMask(GL_TRUE);

  //Draw GUI
  glClear(GL_DEPTH_BUFFER_BIT);
  UiRenderer::render(*this);
}

void Renderer::renderScene(const Camera &camera) {
  setShader(&ShaderLoader::getShader("ambient.frag"));
  renderEntities(camera);

  // Set the light blending to additive, and depth testing to less or equal
  // in order to draw multiple layers of light over our ambient image
  glBlendFuncSeparate(GL_ONE, GL_ONE, GL_ONE, GL_ZERO);
  glDepthFunc(GL_LEQUAL);

  setShader(&ShaderLoader::getShader("diffuse.frag"));
  /* Lights */ {
    for (const Entity &e : scene->entities) {
      if (not e.hasComponent<LightSource>()) {
        continue;
      }

      const Transform &t = e.getComponent<Transform>();
      const LightSource &ls = e.getComponent<LightSource>();

      if (not ls.enabled) {
        continue;
      }

      glUniform3f(shader->uni("light.position"), t.position.x, t.position.y, t.position.z);
      glUniform3f(shader->uni("light.color"), ls.color.x, ls.color.y, ls.color.z);
      glUniform1f(shader->uni("light.distance"), ls.distance);
      glUniform1f(shader->uni("light.energy"), ls.energy);
      glUniform1f(shader->uni("light.specular"), ls.specular);

      renderEntities(camera);
    }
  }

  // Set the blending and depth function back to their normal values
  glDepthFunc(GL_LESS);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Renderer::renderEntities(const Camera &cam) {
  for (Entity &e : scene->entities) {
    if (e.hasComponent<MeshDrawable>()) {
      renderEntity(cam, e);
    }
  }
  renderEntity(cam, scene->end);
}

void Renderer::renderEntity(const Camera &cam, const Entity &e) {
  MeshDrawable &drawable = e.getComponent<MeshDrawable>();
  Matrix4f mtx;
  e.getComponent<Transform>().getModelMtx(mtx);

  renderMesh(cam, mtx, drawable.mesh, drawable.material);
}

void Renderer::renderPlayer(const Camera &cam) {
  const Transform &t = scene->player.getComponent<Transform>();
  Matrix4f mtx;
  mtx.translate(t.position+Vector3f(0, -.5f, 0));
  mtx.rotate(t.rotation.y, 0, 1, 0);
  mtx.scale(Vector3f(1.3f, 1.3f, 1.3f));
  const Mesh &dummy = MeshLoader::getMesh("HumanToken.obj");
  const Material &mat = MaterialLoader::fromTexture("HumanToken.png");

  setShader(&ShaderLoader::getShader("ambient.frag"));
  renderMesh(cam, mtx, dummy, mat);
}

void Renderer::renderPortalStencil(const Camera &cam, const Entity &portal) {
  glClear(GL_STENCIL_BUFFER_BIT);

  // Disable writing to the color and depth buffer
  glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
  glDepthMask(GL_FALSE);

  // Set the stencil function to always fail
  glStencilFunc(GL_NEVER, 1, 0xFF);
  // Replace all stencil pixel where we draw with 1
  glStencilOp(GL_REPLACE, GL_KEEP, GL_KEEP);

  const Portal &p = portal.getComponent<Portal>();
  Matrix4f mtx;
  portal.getComponent<Transform>().applyModelMtx(mtx);
  mtx.scale(p.getScaleMult());

  setShader(&ShaderLoader::getShader("unshaded.frag"));
  renderMesh(cam, mtx, p.stencilMesh);

  // Enable writing to the color and depth buffer again
  glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
  glDepthMask(GL_TRUE);
}

void Renderer::renderPortal(const Camera &cam, const Entity &portal, const Entity &otherPortal) {
  if (portal.getComponent<Portal>().open and otherPortal.getComponent<Portal>().open) {
    glEnable(GL_STENCIL_TEST);

    renderPortalStencil(cam, portal);

    // Set the stencil function so we only render on the stencil
    glStencilFunc(GL_EQUAL, 1, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

    // Draw the whole scene onto the stencil containing the player and recursive portal overlays
    Camera portalCam;
    setCameraInPortal(cam, portalCam, portal, otherPortal);
    renderPlayer(portalCam);
    renderScene(portalCam);
    renderPortalOverlay(portalCam, portal);
    renderPortalOverlay(portalCam, otherPortal);

    glDisable(GL_STENCIL_TEST);
  }
}

void Renderer::renderPortalOverlay(const Camera &cam, const Entity &portal) {
  const Portal &p = portal.getComponent<Portal>();
  if (p.open) {
    setShader(&ShaderLoader::getShader("unshaded.frag"));

    Matrix4f mtx;
    portal.getComponent<Transform>().getModelMtx(mtx);
    mtx.scale(p.getScaleMult());

    renderMesh(cam, mtx, p.overlayMesh, p.overlayTex);
  }
}

void Renderer::renderPortalNoise(const Camera &cam, const Entity &portal, float fade) {
  Matrix4f mtx;
  portal.getComponent<Transform>().getModelMtx(mtx);

  const Portal &p = portal.getComponent<Portal>();
  mtx.scale(p.getScaleMult());

  setShader(&ShaderLoader::getShader("simplexTime.frag"));

  int timeLoc = shader->uni("time");
  int colorLoc = shader->uni("color");
  int noiseAlphaLoc = shader->uni("noiseAlpha");
  glUniform1f(timeLoc, SDL_GetTicks()/1000.f);
  glUniform3f(colorLoc, p.color.x, p.color.y, p.color.z);
  glUniform1f(noiseAlphaLoc, fade);

  renderMesh(cam, mtx, p.overlayMesh, p.maskTex);
}

void Renderer::renderText(const Camera &cam, const std::string &text, Vector3f vector) {
  // FIXME This should be determined by the currently set font
  const Material &mat = MaterialLoader::fromTexture("Pacaya.png");
  setShader(&ShaderLoader::getShader("text.frag"));
  glUniform4f(shader->uni("color"), fontColor.x, fontColor.y, fontColor.z, fontColor.w);
  Vector2f position(vector.x, vector.y);
  Matrix4f mtx;

  const char *array = text.c_str();
  for (unsigned int i = 0; i < text.length(); i++) {
    char c = array[i];

    const Letter &letter = font->getLetter(c);
    const Mesh &mesh = letter.mesh;

    mtx.setIdentity();
    mtx.translate(Vector3f(position.x + letter.xOffset * font->size,
                  position.y + letter.yOffset * font->size,
                  vector.z));

    mtx.scale(Vector3f(letter.width * font->size,
                      letter.height * font->size, 1));

    renderMesh(cam, mtx, mesh, mat);
    position.x += letter.advance * font->size;
  }
}

void Renderer::renderMesh(const Camera &cam, Matrix4f &mdlMtx,
                          const Mesh &mesh, const Material *mat) {
  Matrix4f projMatrix; cam.getProjMatrix(projMatrix);
  glUniformMatrix4fv(shader->uni("projectionMatrix"), 1, false, projMatrix.toArray());
  Matrix4f viewMatrix; cam.getViewMatrix(viewMatrix);
  glUniformMatrix4fv(shader->uni("viewMatrix"), 1, false, viewMatrix.toArray());
  Matrix4f invViewMatrix; cam.getInvViewMatrix(invViewMatrix);
  glUniformMatrix4fv(shader->uni("invViewMatrix"), 1, false, invViewMatrix.toArray());

  Matrix3f mdlMtx3 = toMatrix3f(mdlMtx);
  Matrix3f i3 = inverse(mdlMtx3);
  Matrix4f modelTrInv4Matrix = toMatrix4f(transpose(i3));
  glUniformMatrix4fv(shader->uni("modelTrInv4Matrix"), 1, false, modelTrInv4Matrix.toArray());

  glUniformMatrix4fv(shader->uni("modelMatrix"), 1, false, mdlMtx.toArray());

  glBindVertexArray(mesh.handle);
  if (mat) {
    int tiling = shader->uni("tiling");
    glUniform2f(tiling, mat->scaleU, mat->scaleV);
    glUniform1i(shader->uni("diffuse"), 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mat->diffuse.handle);
    glUniform1i(shader->uni("normalMap"), 1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, mat->normal.handle);
    glUniform1i(shader->uni("specularMap"), 2);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, mat->specular.handle);
    glUniform1f(shader->uni("shininess"), mat->shininess);
  }
  glDrawArrays(GL_TRIANGLES, 0, mesh.numFaces * 3);
  if (mat) {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, 0);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, 0);
  }
  glBindVertexArray(0);
}

void Renderer::setCameraInPortal(const Camera &cam, Camera &dest,
                                 const Entity &portal, const Entity &otherPortal) {
  Transform &p1T = portal.getComponent<Transform>();
  Matrix4f p1mat;
  p1mat.translate(p1T.position);
  p1mat.rotate(p1T.rotation);
  Transform &p2T = otherPortal.getComponent<Transform>();
  Matrix4f p2mat;
  p2mat.translate(p2T.position);
  p2mat.rotate(p2T.rotation);
  Matrix4f rotate180; rotate180.rotate(rad(180), 0, 1, 0);
  Matrix4f view; cam.getViewMatrix(view);
  Matrix4f destView = view * p1mat * rotate180 * inverse(p2mat);

  dest.setPerspective();
  dest.setAspect(cam.getAspect());
  dest.setFovy(cam.getFovy());
  dest.setZNear((p1T.position - cam.getPosition()).length());
  dest.setViewMatrix(destView);
}

/**
 * TODO Documentation required.
 */
Matrix4f Renderer::clipProjMat(const Entity &ent,
                               const Matrix4f &view, const Matrix4f &proj) {
  const Transform &t = ent.getComponent<Transform>();
  Vector4f clipPlane(Math::toDirection(t.rotation),
                     -dot(Math::toDirection(t.rotation),
                          t.position));
  clipPlane = inverse(transpose(view)) * clipPlane;

  if (clipPlane.w > 0.f){
    return proj;
  }

  Vector4f q = inverse(proj) * Vector4f(
    sign(clipPlane.x),
    sign(clipPlane.y),
    1.0f,
    1.0f
  );

  Vector4f c = clipPlane * (2.0f / dot(clipPlane, q));

  Matrix4f newProj = proj;
  newProj[2] = c.x - newProj[3];
  newProj[6] = c.y - newProj[7];
  newProj[10] = c.z - newProj[11];
  newProj[14] = c.w - newProj[15];
  return newProj;
}

} /* namespace glPortal */
