#ifndef CAMERA_H
#define CAMERA_H

#include <engine/core/math/Matrix4f.hpp>
#include <engine/core/math/Vector3f.hpp>

const float DEFAULT_FOVY = 60;
const float DEFAULT_ASPECT = 1;
const float DEFAULT_ZNEAR = 0.1f;
const float DEFAULT_ZFAR = 100;
const bool  DEFAULT_PERSPECTIVE = true;
const float DEFAULT_TOP = 0.5f;
const float DEFAULT_BOTTOM = -0.5f;
const float DEFAULT_LEFT = -0.5f;
const float DEFAULT_RIGHT = 0.5f;

namespace glPortal {

class Camera {
public:
  Camera() {
    recalculate();
  }
  Camera(float fovy, float aspect, float zNear, float zFar);
  void recalculate();
  Matrix4f getProjectionMatrix();
  void setFovy(float fovy);
  void setAspect(float aspect);
  void setZNear(float zNear);
  void setZFar(float zFar);
  void setPerspective();
  void setOrthographic();
  void setLeft(float left);
  void setRight(float right);
  void setTop(float top);
  void setBottom(float bottom);

  Vector3f position;
  Vector3f rotation;
private:
  Matrix4f projectionMatrix;
  bool perspective = DEFAULT_PERSPECTIVE;
  float fovy    = DEFAULT_FOVY;
  float aspect  = DEFAULT_ASPECT;
  float zNear   = DEFAULT_ZNEAR;
  float zFar    = DEFAULT_ZFAR;

  float top     = DEFAULT_TOP;
  float bottom  = DEFAULT_BOTTOM;
  float left    = DEFAULT_LEFT;
  float right   = DEFAULT_RIGHT;
};

} /* namespace glPortal */

#endif /* CAMERA_H */
