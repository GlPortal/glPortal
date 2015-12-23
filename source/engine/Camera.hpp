#ifndef CAMERA_H
#define CAMERA_H

#include <engine/core/math/Math.hpp>
#include <engine/core/math/Matrix4f.hpp>
#include <engine/core/math/Vector3f.hpp>
#include <engine/core/math/Quaternion.hpp>

namespace glPortal {

class Camera {
public:
  static constexpr const float DEFAULT_FOVY = rad(60);
  static constexpr const float DEFAULT_ASPECT = 1;
  static constexpr const float DEFAULT_ZNEAR = 0.1f;
  static constexpr const float DEFAULT_ZFAR = 100;
  static constexpr const bool  DEFAULT_PERSPECTIVE = true;
  static constexpr const float DEFAULT_TOP = 0.5f;
  static constexpr const float DEFAULT_BOTTOM = -0.5f;
  static constexpr const float DEFAULT_LEFT = -0.5f;
  static constexpr const float DEFAULT_RIGHT = 0.5f;

  Camera() {}
  Camera(float fovy, float aspect, float zNear, float zFar);

  void getProjMatrix(Matrix4f &m) const;
  void setProjMatrix(const Matrix4f &m);
  void getViewMatrix(Matrix4f &m) const;
  void setViewMatrix(const Matrix4f &m);
  void getInvViewMatrix(Matrix4f &m) const;

  void setFovy(float fovy);
  float getFovy() const;
  void setAspect(float aspect);
  float getAspect() const;
  void setZNear(float zNear);
  float getZNear() const;
  void setZFar(float zFar);
  float getZFar() const;
  void setPerspective();
  void setOrthographic();
  void setBounds(float left, float right, float bottom, float top);

  Vector3f getPosition() const;
  void setPosition(const Vector3f&);
  Quaternion getOrientation() const;
  void setOrientation(const Quaternion&);

private:
  Vector3f position;
  Quaternion orientation;

  void calcProj();
  void calcView();
  Matrix4f projMatrix;
  Matrix4f viewMatrix;
  Matrix4f invViewMatrix;

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
