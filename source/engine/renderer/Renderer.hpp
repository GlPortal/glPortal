#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <GL/glew.h>
#include <string>

#include <assets/scene/Scene.hpp>
#include <assets/shader/Shader.hpp>
#include <assets/text/Font.hpp>

#include <engine/core/math/Matrix4f.hpp>


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
  
  void setScene(Scene *scene);
  void changeShader(const std::string &path);

  void render();
  void renderScene();
  void renderEntity(const VisualEntity &e);
  void renderPlayer();
  void renderPortalContent(const Portal &portal);
  void renderPortal(const Portal &portal, const Portal &otherPortal);
  void renderPortalOverlay(const Portal &portal);
  void renderPortalNoise(const Portal &portal, float fade);
  void renderText(const std::string &text, int x, int y);
  void renderTexturedMesh(const Mesh &mesh, const Material &mat);
  void setCameraInPortal(const Portal &portal, const Portal &otherPortal);
  void setCameraInPlayer(const Camera& camera);
  void setFont(const std::string &font, float size);
private:
  Viewport *viewport;
  int vpWidth, vpHeight;

  Scene *scene;
  Shader *shader;
  Font *font;
  int portalDepth;
  Matrix4f projectionMatrix;
  Matrix4f viewMatrix;
  Matrix4f modelMatrix;
  
  GLuint modelLoc;
  GLuint normalLoc;
  GLuint viewLoc;
  GLuint projLoc;
};

} /* namespace glPortal */

#endif /* RENDERER_HPP */
