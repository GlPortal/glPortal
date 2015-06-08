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
  Camera() {}
  Camera(float fovy, float aspect, float zNear, float zFar);

  void getProjMatrix(Matrix4f &m) const;
  void setProjMatrix(const Matrix4f &m);
  void getViewMatrix(Matrix4f &m) const;
  void setViewMatrix(const Matrix4f &m);

  void setFovy(float fovy);
  float getFovy() const;
  void setAspect(float aspect);
  float getAspect() const;
  void setZNear(float zNear);
  void setZFar(float zFar);
  void setPerspective();
  void setOrthographic();
  void setBounds(float left, float right, float bottom, float top);

  Vector3f getPosition() const;
  void setPosition(const Vector3f &p);
  Vector3f getRotation() const;
  void setRotation(const Vector3f &r);

private:
  Vector3f position;
  Vector3f rotation;

  void calcProj();
  void calcView();
  Matrix4f projMatrix;
  Matrix4f viewMatrix;

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
