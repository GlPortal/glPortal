#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <string>

#include <assets/scene/Scene.hpp>
#include <assets/shader/Shader.hpp>
#include <assets/text/Font.hpp>

#include <engine/core/math/Matrix4f.hpp>
#include <engine/renderer/UiRenderer.hpp>

namespace glPortal {
class Camera;
class Entity;
class Portal;
class Scene;
class Texture;
class Font;
class Viewport;

class Renderer {
public:
  Renderer();
  void setViewport(Viewport *vp);
  Viewport* getViewport() const;
  void setScene(Scene *scene);
  void setShader(Shader *scene);
  void render();
  void renderScene(const Camera &cam);
  void renderEntities(const Camera &cam);
  void renderEntity(const Camera &cam, const Entity &e);
  void renderPlayer(const Camera &cam);
  void renderPortalContent(const Camera &cam, const Entity &portal);
  void renderPortal(const Camera &cam, const Entity &portal, const Entity &otherPortal);
  void renderPortalOverlay(const Camera &cam, const Entity &portal);
  void renderPortalNoise(const Camera &cam, const Entity &portal, float fade);
  void renderText(const Camera &cam, const std::string &text, int x, int y);
  void renderMesh(const Camera &cam, Matrix4f &mdlMtx, const Mesh &mesh, const Material &mat) {
    return renderMesh(cam, mdlMtx, mesh, &mat);
  }

  void renderMesh(const Camera &cam, Matrix4f &mdlMtx, const Mesh &mesh, const Material *mat = nullptr);
  void renderColoredMesh(const Camera &cam, Matrix4f &mdlMtx, Vector3f color, float alpha);
  void setCameraInPortal(const Camera &cam, Camera &dest, const Entity &portal, const Entity &otherPortal);
  void setFont(const std::string &font, float size);

  static Matrix4f clipProjMat(const Entity &ent, const Matrix4f &view, const Matrix4f &proj);
  Viewport *viewport;
private:
  Camera getCamera();
  int vpWidth, vpHeight;

  Shader *shader;
  Scene *scene;
  Font *font;
  int portalDepth;
};

} /* namespace glPortal */

#endif /* RENDERER_HPP */
