#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <engine/Shader.hpp>
#include <util/Matrix4f.hpp>
#include <GL/glew.h>
#include <string>
#include <engine/Text.hpp>

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
  void renderText(Scene* scene, Text text);
private:
  Shader shader;
  Matrix4f projectionMatrix;
  Matrix4f viewMatrix;
  Matrix4f modelMatrix;
  
  GLuint modelLoc;
  GLuint viewLoc;
  GLuint projLoc;
};

} /* namespace glPortal */

#endif /* RENDERER_HPP */
