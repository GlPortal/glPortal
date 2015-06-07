#include "Camera.hpp"
#include <engine/core/math/Math.hpp>

#include <math.h>
#include "../Window.hpp"

namespace glPortal {

Camera::Camera(float fovy, float aspect, float zNear, float zFar) {
  this->fovy = fovy;
  this->aspect = aspect;
  this->zNear = zNear;
  this->zFar = zFar;
}

void Camera::getProjMatrix(Matrix4f &m) const {
  m.setIdentity();
  if (perspective) {
    float fovyr = Math::toRadians(fovy);
    m[0] = (float) (1 / tan(fovyr / 2)) / aspect;
    m[5] = (float) (1 / tan(fovyr / 2));
    m[10] = (zNear + zFar) / (zNear - zFar);
    m[11] = -1;
    m[14] = (2 * zNear * zFar) / (zNear - zFar);
    m[15] = 0;
  } else {
    m[0] = 2 / (right - left);
    m[5] = 2 / (top - bottom);
    m[10] = -2 / (zFar - zNear);
    m[12] = (left - right) / (right - left);
    m[13] = (bottom - top) / (top - bottom);
    m[14] = (zNear - zFar) / (zFar - zNear);
  }
}

void Camera::getViewMatrix(Matrix4f &m) const {
  m.setIdentity();
  m.rotate(-rotation.x, 1, 0, 0);
  m.rotate(-rotation.y, 0, 1, 0);
  m.translate(negate(position));
}

void Camera::setFovy(float fovy) {
  this->fovy = fovy;
}

void Camera::setAspect(float aspect) {
  this->aspect = aspect;
}

float Camera::getAspect() const {
  return aspect;
}

void Camera::setZNear(float zNear) {
  this->zNear = zNear;
}

void Camera::setZFar(float zFar) {
  this->zFar = zFar;
}

void Camera::setPerspective() {
  this->perspective = true;
}

void Camera::setOrthographic() {
  this->perspective = false;
}

void Camera::setBounds(float left, float right, float bottom, float top) {
  this->left = left;
  this->right = right;
  this->bottom = bottom;
  this->top = top;
}

} /* namespace glPortal */
