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
#include <assets/text/Glyph.hpp>
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
#include <engine/env/Environment.hpp>
#include <Game.hpp>

#include <SDL2/SDL_timer.h>
#include <algorithm>
#include "UiRenderer.hpp"
#include "TerminalRenderer.hpp"

namespace glPortal {

  Renderer::Renderer() : font(nullptr), vpWidth(0), vpHeight(0),
                         scene(nullptr), viewport(nullptr),
                         portalDepth(2), fontColor(1, 1, 1, 1){
}

void Renderer::setViewport(Viewport *vp) {
  viewport = vp;
  viewport->getSize(&vpWidth, &vpHeight);
}

Viewport* Renderer::getViewport() const {
  return viewport;
}

void Renderer::setScene(Scene *scene) {
  this->scene = scene;
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

void Renderer::render(double dtime, const Camera &cam) {
  time += dtime;
  viewport->getSize(&vpWidth, &vpHeight);

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
  camera.setAspect((float)vpWidth / vpHeight);

  Shader &diffuse = ShaderLoader::getShader("diffuse.frag");
  glUseProgram(diffuse.handle);

  /* Lights */ {
    int numLights = 0;
    for (const Entity &e : scene->entities) {
      if (not e.hasComponent<LightSource>()) {
        continue;
      }

      LightSource &ls = e.getComponent<LightSource>();
      if (not ls._uploaded) {
        const Transform &t = e.getComponent<Transform>();
        std::string index = std::to_string(numLights);
        std::string position = "lights[" + index + "].position";
        std::string color = "lights[" + index + "].color";
        std::string distance = "lights[" + index + "].distance";
        std::string energy = "lights[" + index + "].energy";
        std::string specular = "lights[" + index + "].specular";
        const Vector3f &tposition = t.getPosition();
        glUniform3f(diffuse.uni(position.c_str()), tposition.x, tposition.y, tposition.z);
        glUniform3f(diffuse.uni(color.c_str()), ls.color.x, ls.color.y, ls.color.z);
        glUniform1f(diffuse.uni(distance.c_str()), ls.distance);
        glUniform1f(diffuse.uni(energy.c_str()), ls.energy);
        glUniform1f(diffuse.uni(specular.c_str()), ls.specular);
        // ls._uploaded = true;
      }

      ++numLights;
    }
    int numLightsLoc = diffuse.uni("numLights");
    glUniform1i(numLightsLoc, numLights);
  }

  Shader &metal = ShaderLoader::getShader("metal.frag");
  glUseProgram(metal.handle);

  /* Lights */ {
    int numLights = 0;
    for (const Entity &e : scene->entities) {
      if (not e.hasComponent<LightSource>()) {
        continue;
      }

      LightSource &ls = e.getComponent<LightSource>();
      if (not ls._uploaded) {
        const Transform &t = e.getComponent<Transform>();
        std::string index = std::to_string(numLights);
        std::string position = "lights[" + index + "].position";
        std::string color = "lights[" + index + "].color";
        std::string distance = "lights[" + index + "].distance";
        std::string energy = "lights[" + index + "].energy";
        std::string specular = "lights[" + index + "].specular";
        const Vector3f &tposition = t.getPosition();
        glUniform3f(metal.uni(position.c_str()), tposition.x, tposition.y, tposition.z);
        glUniform3f(metal.uni(color.c_str()), ls.color.x, ls.color.y, ls.color.z);
        glUniform1f(metal.uni(distance.c_str()), ls.distance);
        glUniform1f(metal.uni(energy.c_str()), ls.energy);
        glUniform1f(metal.uni(specular.c_str()), ls.specular);
        // ls._uploaded = true;
      }

      ++numLights;
    }
    int numLightsLoc = metal.uni("numLights");
    glUniform1i(numLightsLoc, numLights);
  }

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
      Shader &unshaded = ShaderLoader::getShader("unshaded.frag");

      glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
      glDepthMask(GL_TRUE);
      glClear(GL_DEPTH_BUFFER_BIT);

      const Mesh &portalStencil = MeshLoader::getMesh("PortalStencil.obj");

      Matrix4f mtx;
      pEnt1.getComponent<Transform>().getModelMtx(mtx);
      mtx.scale(portal1.getScaleMult());
      renderMesh(camera, unshaded, mtx, portalStencil);

      pEnt2.getComponent<Transform>().getModelMtx(mtx);
      mtx.scale(portal2.getScaleMult());
      renderMesh(camera, unshaded, mtx, portalStencil);

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
      double dtOpen = scene->world->getTime()-std::max(portal1.openSince, portal2.openSince);
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
  TerminalRenderer::render(*this);
}

void Renderer::renderScene(const Camera &camera) {
  renderEntities(camera);
}

void Renderer::renderEntities(const Camera &cam) {
  for (Entity &e : scene->entities) {
    if (e.hasComponent<MeshDrawable>()) {
      renderEntity(cam, e);
    }
  }
}

void Renderer::renderEntity(const Camera &cam, const Entity &e) {
  MeshDrawable &drawable = e.getComponent<MeshDrawable>();
  Matrix4f mtx;
  e.getComponent<Transform>().getModelMtx(mtx);

  if (drawable.material.fancyname.compare("Metal tiles .5x") == 0) {
    Shader &metal = ShaderLoader::getShader("metal.frag");
    renderMesh(cam, metal, mtx, drawable.mesh, drawable.material);
  } else {
    Shader &diffuse = ShaderLoader::getShader("diffuse.frag");
    renderMesh(cam, diffuse, mtx, drawable.mesh, drawable.material);
  }
}

void Renderer::renderPlayer(const Camera &cam) {
  const Transform &t = scene->player->getComponent<Transform>();
  Matrix4f mtx;
  mtx.translate(t.getPosition() + Vector3f(0, -.5f, 0));
  mtx.rotate(t.getOrientation());
  mtx.scale(Vector3f(1.3f, 1.3f, 1.3f));
  const Mesh &dummy = MeshLoader::getMesh("HumanToken.obj");
  const Material &mat = MaterialLoader::fromTexture("HumanToken.png");

  renderMesh(cam, ShaderLoader::getShader("diffuse.frag"), mtx, dummy, mat);
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

  renderMesh(cam, ShaderLoader::getShader("unshaded.frag"), mtx, p.stencilMesh);

  // Enable writing to the color and depth buffer again
  glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
  glDepthMask(GL_TRUE);
}

void Renderer::renderPortal(const Camera &cam, const Entity &portal, const Entity &otherPortal) {
  if (portal.getComponent<Portal>().open and otherPortal.getComponent<Portal>().open) {
    Transform &pt = portal.getComponent<Transform>();
    Transform &opt = otherPortal.getComponent<Transform>();
    Vector3f displacement = pt.getPosition() - opt.getPosition();
    int recursionLevel = Environment::getConfig().getRecursionLevel();
    Camera portalCam;

    Vector3f portalPosition = pt.getPosition();
    Quaternion portalOrientation = pt.getOrientation();
    Vector3f portalScale = pt.getScale();
    Vector3f portalDir = portal.getComponent<Portal>().getDirection();
    Vector3f otherPortalDir = otherPortal.getComponent<Portal>().getDirection();
    float dotProduct = dot(portalDir, otherPortalDir);

    if (abs(dotProduct) < 0.0001) {
      // The two portals' direction are perpendicular to each other
      Quaternion RotDifference = conjugate(pt.getOrientation() * conjugate(opt.getOrientation()));
      displacement = toMatrix3f(RotDifference.toMatrix()).transform(displacement);

      // recursionLevel is set to 1, because we can't see past 1 level
      // when the portals are perpendicular to each other
      recursionLevel = 1;

      pt.setOrientation(opt.getOrientation());
    } else if (dotProduct == 1.0f) {
      // The two portals are facing the same direction
      recursionLevel = 0;
    }

    // Render portal from the deepest recursion level up for recursionLevel > 0
    pt.setPosition(pt.getPosition() + displacement * (float)recursionLevel);
    for (int i = 0; i < recursionLevel; i++) {
      glEnable(GL_STENCIL_TEST);
      glClear(GL_STENCIL_BUFFER_BIT);
      glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
      glDepthMask(GL_FALSE);

      renderPortalStencil(cam, portal);

      glStencilFunc(GL_EQUAL, 1, 0xFF);
      glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

      setCameraInPortal(cam, portalCam, portal, otherPortal);
      renderPlayer(portalCam);
      renderScene(portalCam);

      // Restore portal to its original position, then render the overlay,
      // so that the overlay will show up correctly
      Vector3f tmpPosition = pt.getPosition();
      pt.setPosition(portalPosition);
      renderPortalOverlay(portalCam, portal);
      pt.setPosition(tmpPosition);

      // Move the displaced portal one level back
      pt.setPosition(pt.getPosition() - displacement);
      // The next line only matters if the portals are perpendicular to each other
      // In all other cases, the displaced portal's orientation doesn't change
      pt.setOrientation(portalOrientation);
      setCameraInPortal(cam, portalCam, portal, otherPortal);
      glDisable(GL_STENCIL_TEST);

      // Draw a "depth lid" over the portal scene that was just rendered so that the content in the
      // portal now have the same depth value as the surface the portal is drawn on
      Shader &unshaded = ShaderLoader::getShader("unshaded.frag");
      glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
      glDepthMask(GL_TRUE);
      glClear(GL_DEPTH_BUFFER_BIT);
      const Mesh &portalStencil = MeshLoader::getMesh("PortalStencil.obj");
      Matrix4f mtx;
      // Use portal's original transform so that the depth values are consistent
      // with the surrounding scene
      mtx.translate(portalPosition);
      mtx.rotate(portalOrientation);
      mtx.scale(portalScale);
      mtx.scale(portal.getComponent<Portal>().getScaleMult());
      renderMesh(portalCam, unshaded, mtx, portalStencil);
      glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    }

    // Render portal at the default level
    glEnable(GL_STENCIL_TEST);

    renderPortalStencil(cam, portal);

    // Set the stencil function so we only render on the stencil
    glStencilFunc(GL_EQUAL, 1, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

    // Draw the whole scene onto the stencil containing the player and recursive portal overlays
    setCameraInPortal(cam, portalCam, portal, otherPortal);
    renderPlayer(portalCam);
    renderScene(portalCam);
    renderPortalOverlay(portalCam, portal);
    glDisable(GL_STENCIL_TEST);
  }
}

void Renderer::renderPortalOverlay(const Camera &cam, const Entity &portal) {
  const Portal &p = portal.getComponent<Portal>();
  if (p.open) {
    Matrix4f mtx;
    portal.getComponent<Transform>().getModelMtx(mtx);
    mtx.scale(p.getScaleMult());

    renderMesh(cam, ShaderLoader::getShader("unshaded.frag"), mtx, p.overlayMesh, p.overlayTex);
  }
}

void Renderer::renderPortalNoise(const Camera &cam, const Entity &portal, float fade) {
  Matrix4f mtx;
  portal.getComponent<Transform>().getModelMtx(mtx);

  const Portal &p = portal.getComponent<Portal>();
  mtx.scale(p.getScaleMult());

  Shader &simplexTime = ShaderLoader::getShader("simplexTime.frag");
  glUseProgram(simplexTime.handle);
  int timeLoc = simplexTime.uni("time");
  int colorLoc = simplexTime.uni("color");
  int noiseAlphaLoc = simplexTime.uni("noiseAlpha");
  glUniform1f(timeLoc, scene->world->getTime());
  glUniform3f(colorLoc, p.color.x, p.color.y, p.color.z);
  glUniform1f(noiseAlphaLoc, fade);

  renderMesh(cam, simplexTime, mtx, p.overlayMesh, p.maskTex);
}

void Renderer::renderText(const Camera &cam, const std::string &text, Vector3f vector) {
  // FIXME This should be determined by the currently set font
  const Material &mat = MaterialLoader::fromTexture("Pacaya.png");
  Shader &shader = ShaderLoader::getShader("text.frag");
  glUniform4f(shader.uni("color"), fontColor.x, fontColor.y, fontColor.z, fontColor.w);
  Vector2f position(vector.x, vector.y);
  Matrix4f mtx;

  const char *array = text.c_str();
  for (unsigned int i = 0; i < text.length(); i++) {
    char c = array[i];

    const Glyph &letter = font->getGlyph(c);
    const Mesh &mesh = letter.mesh;

    mtx.setIdentity();
    mtx.translate(Vector3f(position.x + letter.xOffset * font->size,
                  position.y + letter.yOffset * font->size,
                  vector.z));

    mtx.scale(Vector3f(letter.width * font->size,
                      letter.height * font->size, 1));

    renderMesh(cam, shader, mtx, mesh, mat);
    position.x += letter.advance * font->size;
  }
}


void Renderer::renderMesh(const Camera &cam, Shader &shader, Matrix4f &mdlMtx,
                          const Mesh &mesh, const Material *mat) {
  glUseProgram(shader.handle);

  Matrix4f projMatrix; cam.getProjMatrix(projMatrix);
  glUniformMatrix4fv(shader.uni("projectionMatrix"), 1, false, projMatrix.toArray());
  Matrix4f viewMatrix; cam.getViewMatrix(viewMatrix);
  glUniformMatrix4fv(shader.uni("viewMatrix"), 1, false, viewMatrix.toArray());
  Matrix4f invViewMatrix = inverse(viewMatrix);
  glUniformMatrix4fv(shader.uni("invViewMatrix"), 1, false, invViewMatrix.toArray());

  Matrix3f mdlMtx3 = toMatrix3f(mdlMtx);
  Matrix3f i3 = inverse(mdlMtx3);
  Matrix4f modelTrInv4Matrix = toMatrix4f(transpose(i3));
  glUniformMatrix4fv(shader.uni("modelTrInv4Matrix"), 1, false, modelTrInv4Matrix.toArray());

  glUniformMatrix4fv(shader.uni("modelMatrix"), 1, false, mdlMtx.toArray());

  // Per-vertex color multiplier
  glVertexAttrib4f(4, 1, 1, 1, 1);

  glBindVertexArray(mesh.handle);
  if (mat) {
    int tiling = shader.uni("tiling");
    glUniform2f(tiling, mat->scaleU, mat->scaleV);
    glUniform1i(shader.uni("diffuse"), 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mat->diffuse.handle);
    glUniform1i(shader.uni("normalMap"), 1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, mat->normal.handle);
    glUniform1i(shader.uni("specularMap"), 2);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, mat->specular.handle);
    glUniform1f(shader.uni("shininess"), mat->shininess);
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
  p1mat.translate(p1T.getPosition());
  p1mat.rotate(p1T.getOrientation());
  Transform &p2T = otherPortal.getComponent<Transform>();
  Matrix4f p2mat;
  p2mat.translate(p2T.getPosition());
  p2mat.rotate(p2T.getOrientation());
  Matrix4f rotate180; rotate180.rotate(rad(180), 0, 1, 0);
  Matrix4f view; cam.getViewMatrix(view);
  Matrix4f destView = view * p1mat * rotate180 * inverse(p2mat);

  dest.setPerspective();
  dest.setAspect(cam.getAspect());
  dest.setFovy(cam.getFovy());
  dest.setZNear((p1T.getPosition() - cam.getPosition()).length());
  dest.setViewMatrix(destView);
}

/**
 * TODO Documentation required.
 */
/*
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
*/

} /* namespace glPortal */
