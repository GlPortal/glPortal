#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "util/Matrix4f.hpp"
#include "../Scene.hpp"

namespace glPortal {
class Renderer {
public:
  Renderer();
  void render(Scene* scene);
  Mesh getBox(Entity wall);
private:
  int shader;
  Matrix4f projectionMatrix;
  Matrix4f viewMatrix;
  Matrix4f modelMatrix;
};

} /* namespace glPortal */

#endif /* RENDERER_HPP */
