#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <GL/glew.h>
#include <string>

#include "Shader.hpp"
#include "util/Matrix4f.hpp"

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
