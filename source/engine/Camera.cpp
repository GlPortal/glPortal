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

void Camera::loadMatrix(Matrix4f& m) const {
  m.setIdentity();
  if (perspective) {
    float fovyr = Math::toRadians(fovy);
    m.array[0] = (float) (1 / tan(fovyr / 2)) / aspect;
    m.array[5] = (float) (1 / tan(fovyr / 2));
    m.array[10] = (zNear + zFar) / (zNear - zFar);
    m.array[11] = -1;
    m.array[14] = (2 * zNear * zFar) / (zNear - zFar);
    m.array[15] = 0;
  } else {
    m.array[0] = 2 / (right - left);
    m.array[5] = 2 / (top - bottom);
    m.array[10] = -2 / (zFar - zNear);
    m.array[12] = (left - right) / (right - left);
    m.array[13] = (bottom - top) / (top - bottom);
    m.array[14] = (zNear - zFar) / (zFar - zNear);
  }
}

void Camera::setFovy(float fovy) {
  this->fovy = fovy;
}

void Camera::setAspect(float aspect) {
  this->aspect = aspect;
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
