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

namespace glPortal {

Renderer::Renderer() : viewport(nullptr), portalDepth(2) {
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

/**
 * Sets the current scene for rendering
 */
void Renderer::setScene(Scene *scene) {
  this->scene = scene;
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


void Renderer::render(double dtime, const Camera &cam) {
  time += dtime;
  viewport->getSize(&vpWidth, &vpHeight);

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  Camera &camera = scene->camera;
  camera.setPerspective();
  camera.setAspect((float)vpWidth / vpHeight);

  const Shader &diffuse = ShaderLoader::getShader("diffuse.frag");
  glUseProgram(diffuse.handle);

  /* Lights */ {
    int numLights = 0;
    for (const Entity &e : scene->entities) {
      if (not e.hasComponent<LightSource>()) {
        continue;
      }

      LightSource &ls = e.getComponent<LightSource>();
      if (!ls._uploaded) {
        const Transform &t = e.getComponent<Transform>();
        std::string index = std::to_string(numLights);
        std::string position = "lights[" + index + "].position";
        std::string color = "lights[" + index + "].color";
        std::string distance = "lights[" + index + "].distance";
        std::string energy = "lights[" + index + "].energy";
        std::string specular = "lights[" + index + "].specular";
        glUniform3f(diffuse.uni(position.c_str()), t.position.x, t.position.y, t.position.z);
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

  const Shader &metal = ShaderLoader::getShader("metal.frag");
  glUseProgram(metal.handle);

  /* Lights */ {
    int numLights = 0;
    for (const Entity &e : scene->entities) {
      if (not e.hasComponent<LightSource>()) {
        continue;
      }

      LightSource &ls = e.getComponent<LightSource>();
      if (!ls._uploaded) {
        const Transform &t = e.getComponent<Transform>();
        std::string index = std::to_string(numLights);
        std::string position = "lights[" + index + "].position";
        std::string color = "lights[" + index + "].color";
        std::string distance = "lights[" + index + "].distance";
        std::string energy = "lights[" + index + "].energy";
        std::string specular = "lights[" + index + "].specular";
        glUniform3f(metal.uni(position.c_str()), t.position.x, t.position.y, t.position.z);
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
    renderPortal(cam, pEnt1, pEnt2);
    renderPortal(cam, pEnt2, pEnt1);
    // Depth buffer
    if (portal1.open and portal2.open) {
      const Shader &unshaded = ShaderLoader::getShader("unshaded.frag");

      glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
      glDepthMask(GL_TRUE);
      glClear(GL_DEPTH_BUFFER_BIT);

      const Mesh &portalStencil = MeshLoader::getMesh("PortalStencil.obj");

      Matrix4f mtx;
      pEnt1.getComponent<Transform>().getModelMtx(mtx);
      mtx.scale(portal1.getScaleMult());
      renderMesh(cam, unshaded, mtx, portalStencil);

      pEnt2.getComponent<Transform>().getModelMtx(mtx);
      mtx.scale(portal2.getScaleMult());
      renderMesh(cam, unshaded, mtx, portalStencil);

      glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    }
  }

  renderScene(cam);

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
        renderPortalNoise(cam, pEnt1, al);
        renderPortalNoise(cam, pEnt2, al);
      }
    } else {
      if (portal1.open) {
        renderPortalNoise(cam, pEnt1, 1.f);
      }
      if (portal2.open) {
        renderPortalNoise(cam, pEnt2, 1.f);
      }
    }
    
    // Draw overlays
    renderPortalOverlay(cam, pEnt1);
    renderPortalOverlay(cam, pEnt2);
  }
  glDepthMask(GL_TRUE);

  //Draw GUI
  glClear(GL_DEPTH_BUFFER_BIT);
  Camera orthoCam;
  orthoCam.setOrthographic();
  orthoCam.setBounds(0, vpWidth, 0, vpHeight);
  renderUI(orthoCam);
}

void Renderer::renderUI(const Camera &cam) {
  // Crosshair
  Matrix4f crosshairMtx;
  crosshairMtx.translate(Vector3f(vpWidth/2, vpHeight/2, -2));
  crosshairMtx.scale(Vector3f(80, 80, 1));
  const Mesh &mesh = MeshLoader::getMesh("GUIElement.obj");
  const Material &mat = MaterialLoader::fromTexture("Reticle.png");
  const Shader &unshaded = ShaderLoader::getShader("unshaded.frag");
  renderMesh(cam, unshaded, crosshairMtx, mesh, mat);

  // Title
  setFont("Pacaya", 1.5f);
  renderText(cam, "GlPortal", 25, vpHeight - 75);

  // FPS counter
  setFont("Pacaya", 0.5f);
  renderText(cam, std::string("FPS: ") + std::to_string(Game::fps.getFps()), 10, vpHeight - 25);
}

void Renderer::renderScene(const Camera &cam) {
  for (Entity &e : scene->entities) {
    if (e.hasComponent<MeshDrawable>()) {
      renderEntity(cam, e);
    }
  }
  renderEntity(cam, *scene->end);
}

void Renderer::renderEntity(const Camera &cam, const Entity &e) {
  MeshDrawable &drawable = e.getComponent<MeshDrawable>();
  Matrix4f mtx;
  e.getComponent<Transform>().getModelMtx(mtx);
  const Shader &diffuse = ShaderLoader::getShader("diffuse.frag");
  const Shader &metal = ShaderLoader::getShader("metal.frag");
  if (drawable.material.fancyname.compare("Metal tiles .5x") == 0) {
    renderMesh(cam, metal, mtx, drawable.mesh, drawable.material);
  } else {
    renderMesh(cam, diffuse, mtx, drawable.mesh, drawable.material);
  }
}

void Renderer::renderPlayer(const Camera &cam) {
  const Transform &t = scene->player->getComponent<Transform>();
  Matrix4f mtx;
  mtx.translate(t.position+Vector3f(0, -.5f, 0));
  mtx.rotate(t.orientation);
  mtx.scale(Vector3f(1.3f, 1.3f, 1.3f));
  const Mesh &dummy = MeshLoader::getMesh("HumanToken.obj");
  const Material &mat = MaterialLoader::fromTexture("HumanToken.png");
  const Shader &diffuse = ShaderLoader::getShader("diffuse.frag");
  renderMesh(cam, diffuse, mtx, dummy, mat);
}

void Renderer::renderPortalContent(const Camera &cam, const Entity &portal) {
  //Stencil drawing
  //Primary portal
  glStencilFunc(GL_NEVER, 1, 0xFF);
  glStencilOp(GL_REPLACE, GL_KEEP, GL_KEEP);
  
  const Portal &p = portal.getComponent<Portal>();
  Matrix4f mtx;
  mtx.translate(p.getDirection() * -cam.getZNear());
  portal.getComponent<Transform>().applyModelMtx(mtx);
  mtx.scale(p.getScaleMult());
  renderMesh(cam, ShaderLoader::getShader("unshaded.frag"), mtx, p.stencilMesh);

  mtx.setIdentity();
  portal.getComponent<Transform>().applyModelMtx(mtx);
  mtx.scale(p.getScaleMult());
  renderMesh(cam, ShaderLoader::getShader("unshaded.frag"), mtx, p.stencilMesh);

  glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
  glDepthMask(GL_TRUE);
}

void Renderer::renderPortal(const Camera &cam, const Entity &portal, const Entity &otherPortal) {
  if (portal.getComponent<Portal>().open and otherPortal.getComponent<Portal>().open) {
    glEnable(GL_STENCIL_TEST);
    glClear(GL_STENCIL_BUFFER_BIT);
    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
    glDepthMask(GL_FALSE);

    renderPortalContent(cam, portal);
    
    //Draw the scene from the secondary portal
    glStencilFunc(GL_EQUAL, 1, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

    Camera portalCam;
    setCameraInPortal(cam, portalCam, portal, otherPortal);
    // TODO: rework to allow recursive rendering, and draw other portal pairs
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
    const Shader &unshaded = ShaderLoader::getShader("unshaded.frag");
    Matrix4f mtx;
    portal.getComponent<Transform>().getModelMtx(mtx);
    mtx.scale(p.getScaleMult());

    renderMesh(cam, unshaded, mtx, p.overlayMesh, p.overlayTex);
  }
}

void Renderer::renderPortalNoise(const Camera &cam, const Entity &portal, float fade) {
  Matrix4f mtx;
  portal.getComponent<Transform>().getModelMtx(mtx);

  const Portal &p = portal.getComponent<Portal>();
  mtx.scale(p.getScaleMult());

  const Shader &simplexTime = ShaderLoader::getShader("simplexTime.frag");
  glUseProgram(simplexTime.handle);
  int timeLoc = simplexTime.uni("time");
  int colorLoc = simplexTime.uni("color");
  int noiseAlphaLoc = simplexTime.uni("noiseAlpha");
  glUniform1f(timeLoc, scene->world->getTime());
  glUniform3f(colorLoc, p.color.x, p.color.y, p.color.z);
  glUniform1f(noiseAlphaLoc, fade);

  renderMesh(cam, simplexTime, mtx, p.overlayMesh, p.maskTex);
}

void Renderer::renderText(const Camera &cam, const std::string &text, int x, int y) {
  glClear(GL_DEPTH_BUFFER_BIT);

  const Material &mat = MaterialLoader::fromTexture("Pacaya.png");
  const Shader &sh = ShaderLoader::getShader("text.frag");

  Vector2f position(x, y);
  Matrix4f mtx;

  const char *array = text.c_str();
  for (unsigned int i = 0; i < text.length(); i++) {
    char c = array[i];

    const Letter &letter = font->getLetter(c);
    const Mesh &mesh = letter.mesh;

    mtx.setIdentity();
    mtx.translate(Vector3f(position.x + letter.xOffset * font->size,
                  position.y + letter.yOffset * font->size,
                  -10));

    mtx.scale(Vector3f(letter.width * font->size,
                      letter.height * font->size, 1));

    renderMesh(cam, sh, mtx, mesh, mat);
    position.x += letter.advance * font->size;
  }
}


/**
 * Renders a mesh with the specified texture
 * @param mesh The mesh to render
 */
void Renderer::renderMesh(const Camera &cam, const Shader &sh, Matrix4f &mdlMtx, const Mesh &mesh,
                          const Material *mat) {
  glUseProgram(sh.handle);

  Matrix4f projMatrix; cam.getProjMatrix(projMatrix);
  glUniformMatrix4fv(sh.uni("projectionMatrix"), 1, false, projMatrix.toArray());
  Matrix4f viewMatrix; cam.getViewMatrix(viewMatrix);
  glUniformMatrix4fv(sh.uni("viewMatrix"), 1, false, viewMatrix.toArray());
  Matrix4f invViewMatrix = inverse(viewMatrix);
  glUniformMatrix4fv(sh.uni("invViewMatrix"), 1, false, invViewMatrix.toArray());

  Matrix3f mdlMtx3 = toMatrix3f(mdlMtx);
  Matrix3f i3 = inverse(mdlMtx3);
  Matrix4f modelTrInv4Matrix = toMatrix4f(transpose(i3));
  glUniformMatrix4fv(sh.uni("modelTrInv4Matrix"), 1, false, modelTrInv4Matrix.toArray());

  glUniformMatrix4fv(sh.uni("modelMatrix"), 1, false, mdlMtx.toArray());

  // Per-vertex color multiplier
  glVertexAttrib4f(4, 1, 1, 1, 1);

  glBindVertexArray(mesh.handle);
  if (mat) {
    int tiling = sh.uni("tiling");
    glUniform2f(tiling, mat->scaleU, mat->scaleV);
    glUniform1i(sh.uni("diffuse"), 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mat->diffuse.handle);
    glUniform1i(sh.uni("normalMap"), 1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, mat->normal.handle);
    glUniform1i(sh.uni("specularMap"), 2);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, mat->specular.handle);
    glUniform1f(sh.uni("shininess"), mat->shininess);
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

void Renderer::setCameraInPortal(const Camera &cam, Camera &dest, const Entity &portal,
                                 const Entity &otherPortal) {
  Transform &p1T = portal.getComponent<Transform>();
  Matrix4f p1mat;
  p1mat.translate(p1T.position);
  p1mat.rotate(p1T.orientation);
  Transform &p2T = otherPortal.getComponent<Transform>();
  Matrix4f p2mat;
  p2mat.translate(p2T.position);
  p2mat.rotate(p2T.orientation);
  Matrix4f rotate180; rotate180.rotate(rad(180), 0, 1, 0);
  Matrix4f view; cam.getViewMatrix(view);
  Matrix4f destView = view * p1mat * rotate180 * inverse(p2mat);

  dest.setPerspective();
  dest.setAspect(cam.getAspect());
  dest.setFovy(cam.getFovy());
  dest.setZNear((p1T.position - cam.getPosition()).length());
  //Matrix4f proj; dest.getProjMatrix(proj);
  //dest.setProjMatrix(clipProjMat(portal, destView, proj));
  dest.setViewMatrix(destView);
}

/*
static float sign(float v) {
  if (v > 0.f) return 1.f;
  if (v < 0.f) return -1.f;
  return 0.f;
}

Matrix4f Renderer::clipProjMat(const Entity &ent, const Matrix4f &view, const Matrix4f &proj) {
  const Transform &t = ent.getComponent<Transform>();
  Vector4f clipPlane(Math::toDirection(t.rotation), -dot(Math::toDirection(t.rotation), t.position));
  clipPlane = inverse(transpose(view)) * clipPlane;

  if (clipPlane.w > 0.f)
    return proj;

  Vector4f q = inverse(proj) * Vector4f(
    sign(clipPlane.x),
    sign(clipPlane.y),
    1.0f,
    1.0f
  );

  Vector4f c = clipPlane * (2.0f / dot(clipPlane, q));

  // third row = clip plane - fourth row
  Matrix4f newProj = proj;
  newProj[2] = c.x - newProj[3];
  newProj[6] = c.y - newProj[7];
  newProj[10] = c.z - newProj[11];
  newProj[14] = c.w - newProj[15];
  return newProj;
}
*/

} /* namespace glPortal */
