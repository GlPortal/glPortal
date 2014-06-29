#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <GL/glew.h>

#include "util/Matrix4f.hpp"

namespace glPortal {

class Portal;
class Scene;

class Renderer {
public:
  Renderer();
  void render(Scene* scene);
  void renderPortal(Scene* scene, Portal portal, Portal otherPortal);
  void renderPortalOverlay(Portal portal);
  void renderScene(Scene* scene);
private:
  int shader;
  Matrix4f projectionMatrix;
  Matrix4f viewMatrix;
  Matrix4f modelMatrix;

  GLuint modelLoc;
  GLuint viewLoc;
  GLuint projLoc;
};

} /* namespace glPortal */

#endif /* RENDERER_HPP */
