#ifndef CAMERA_H
#define CAMERA_H

#include "util/Matrix4f.hpp"
#include "util/Vector3f.hpp"

const float DEFAULT_FOVY = 60;
const float DEFAULT_ASPECT = 1;
const float DEFAULT_ZNEAR = 0.1f;
const float DEFAULT_ZFAR = 100;

namespace glPortal {

class Camera {
public:
  Camera() :
      fovy(DEFAULT_FOVY), aspect(DEFAULT_ASPECT), zNear(DEFAULT_ZNEAR), zFar(DEFAULT_ZFAR) {
    recalculate();
  }
  Camera(float fovy, float aspect, float zNear, float zFar);
  void recalculate();
  Matrix4f getProjectionMatrix();
  void setFovy(float fovy);
  void setAspect(float aspect);
  void setZNear(float zNear);
  void setZFar(float zFar);

  Vector3f position;
  Vector3f rotation;
private:
  Matrix4f projectionMatrix;
  float fovy;
  float aspect;
  float zNear;
  float zFar;
};

} /* namespace glPortal */

#endif /* CAMERA_H */
