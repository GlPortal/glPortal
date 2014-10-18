#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <engine/Font.hpp>
#include <engine/Shader.hpp>
#include <GL/glew.h>
#include <util/math/Matrix4f.hpp>
#include <string>

namespace glPortal {
class Texture;
} /* namespace glPortal */

namespace glPortal {
class Font;
} /* namespace glPortal */

namespace glPortal {
class Entity;
class Portal;
class Scene;

class Renderer {
public:
  Renderer();
  void changeShader(std::string path);
  void render(Scene* scene);
  void renderScene(Scene* scene);
  void renderEntity(Entity e);
  void renderPortal(Scene* scene, Portal portal, Portal otherPortal);
  void renderPortalOverlay(Portal portal);
  void renderText(Scene* scene, int x, int y, std::string text);
  void renderTexturedMesh(Mesh mesh, Texture texture);
  void setCameraInPortal(Scene* scene, Portal portal, Portal otherPortal);
  void setFont(std::string font, float size);
private:
  Shader shader;
  Matrix4f projectionMatrix;
  Matrix4f viewMatrix;
  Matrix4f modelMatrix;
  
  GLuint modelLoc;
  GLuint viewLoc;
  GLuint projLoc;

  Font font;
};

} /* namespace glPortal */

#endif /* RENDERER_HPP */
