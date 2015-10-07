#ifndef PHYSICS_DEBUG_DRAW_HPP
#define PHYSICS_DEBUG_DRAW_HPP

#include <memory>
#include <vector>
#include <epoxy/gl.h>
#include <bullet/LinearMath/btIDebugDraw.h>
#include <engine/core/gl/VBO.hpp>
#include <engine/Camera.hpp>

namespace glPortal {

class PhysicsDebugDraw : public btIDebugDraw {
private:
  int dbgMode;
  std::unique_ptr<VBO> vbo;
  struct PtData {
    float x, y, z, r, g, b;
  };
  std::vector<PtData> points;

public:
  PhysicsDebugDraw();

  void drawLine(const btVector3 &from, const btVector3 &to, const btVector3 &color);
  void drawLine(const btVector3 &from, const btVector3 &to, const btVector3 &fromColor,
                const btVector3 &toColor);
  void reportErrorWarning(const char *warningString);
  void draw3dText(const btVector3 &location, const char *textString);
  void drawContactPoint(const btVector3 &PointOnB, const btVector3 &normalOnB,
    btScalar distance, int lifeTime, const btVector3 &color);
  void setDebugMode(int debugMode);
  int getDebugMode() const;

  void render(const Camera &cam);
};

} /* namespace glPortal */

#endif /* PHYSICS_DEBUG_DRAW_HPP */
