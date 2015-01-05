#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <GL/glew.h>
#include <string>

#include <assets/scene/Scene.hpp>
#include <assets/shader/Shader.hpp>
#include <assets/text/Font.hpp>

#include <engine/core/math/Matrix4f.hpp>


namespace glPortal {
class Entity;
class Portal;
class Scene;
class Texture;
class Font;

class Renderer {
public:
  Renderer();
  void setScene(Scene* scene);
  void changeShader(std::string path);

  void render();
  void renderScene();
  void renderEntity(const Entity& e);
  void renderPortal(const Portal& portal, const Portal& otherPortal);
  void renderPortalOverlay(const Portal& portal);
  void renderText(std::string text, int x, int y);
  void renderTexturedMesh(const Mesh& mesh, const Texture& texture);
  void setCameraInPortal(const Portal& portal, const Portal& otherPortal);
  void setFont(std::string font, float size);
private:
  Scene* scene;
  Shader shader;
  Font font;

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
