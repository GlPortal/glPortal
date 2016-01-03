#include "PhysicsDebugDraw.hpp"
#include <assets/shader/ShaderLoader.hpp>
#include <assets/texture/TextureLoader.hpp>
#include <engine/env/System.hpp>

namespace glPortal {

PhysicsDebugDraw::PhysicsDebugDraw() {
  setDebugMode(btIDebugDraw::DBG_DrawWireframe | btIDebugDraw::DBG_DrawContactPoints);
}

void PhysicsDebugDraw::draw3dText(const btVector3 &location, const char *textString) {
  // TODO
}

int PhysicsDebugDraw::getDebugMode() const {
  return dbgMode;
}

void PhysicsDebugDraw::setDebugMode(int debugMode) {
  dbgMode = debugMode;
}

void PhysicsDebugDraw::reportErrorWarning(const char *warningString) {
  System::Log(Warning) << "PhysDbg: " << warningString;
}

void PhysicsDebugDraw::drawLine(const btVector3 &from,
                                const btVector3 &to,
                                const btVector3 &color) {
  points.emplace_back(PtData{from.x(), from.y(), from.z(), color.x(), color.y(), color.z()});
  points.emplace_back(PtData{to.x(), to.y(), to.z(), color.x(), color.y(), color.z()});
}

void PhysicsDebugDraw::drawLine(const btVector3& from, const btVector3& to,
  const btVector3& fromColor, const btVector3& toColor) {
  points.emplace_back(PtData{from.x(), from.y(), from.z(), fromColor.x(), fromColor.y(),
        fromColor.z()});
  points.emplace_back(PtData{to.x(), to.y(), to.z(), toColor.x(), toColor.y(), toColor.z()});
}

void PhysicsDebugDraw::drawContactPoint(const btVector3 &PointOnB, const btVector3 &normalOnB,
  btScalar distance, int lifeTime, const btVector3 &color) {
  drawLine(PointOnB, PointOnB+normalOnB, color);
}


void PhysicsDebugDraw::render(const Camera &cam) {
  if (points.size() == 0) {
    return;
  }
  if (not vbo) {
    vbo = std::make_unique<VBO>(12*4*sizeof(PtData));
  }
  Shader &sh = ShaderLoader::getShader("unshaded.frag");
  glUseProgram(sh.handle);
  Matrix4f projMatrix; cam.getProjMatrix(projMatrix);
  glUniformMatrix4fv(sh.uni("projectionMatrix"), 1, false, projMatrix.toArray());
  Matrix4f viewMatrix; cam.getViewMatrix(viewMatrix);
  glUniformMatrix4fv(sh.uni("viewMatrix"), 1, false, viewMatrix.toArray());
  Matrix4f modelMatrix; // identity
  glUniformMatrix4fv(sh.uni("modelMatrix"), 1, false, modelMatrix.toArray());

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, TextureLoader::getEmptyDiffuse().handle);
  glUniform1i(sh.uni("diffuse"), 0);

  if (vbo->getSize() < points.size()*sizeof(PtData)) {
    vbo->update(points);
  } else {
    vbo->setData(points, GL_DYNAMIC_DRAW);
  }
  vbo->bind();
  glVertexAttribPointer(0, 3, GL_FLOAT, 0, sizeof(PtData), 0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(4, 3, GL_FLOAT, 0, sizeof(PtData), (GLvoid*)(3*sizeof(float)));
  glEnableVertexAttribArray(4);
  glDrawArrays(GL_LINES, 0, points.size());
  points.clear();
}

} /* namespace glPortal */
