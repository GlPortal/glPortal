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
  recalculate();
}

void Camera::recalculate() {
  float aspect = (float) Window::width / Window::height;

  projectionMatrix.setIdentity();
  if (perspective) {
    float fovyr = Math::toRadians(fovy);
    projectionMatrix.array[0] = (float) (1 / tan(fovyr / 2)) / aspect;
    projectionMatrix.array[5] = (float) (1 / tan(fovyr / 2));
    projectionMatrix.array[10] = (zNear + zFar) / (zNear - zFar);
    projectionMatrix.array[11] = -1;
    projectionMatrix.array[14] = (2 * zNear * zFar) / (zNear - zFar);
    projectionMatrix.array[15] = 0;
  } else {
    projectionMatrix.array[0] = 2 / (right - left);
    projectionMatrix.array[5] = 2 / (top - bottom);
    projectionMatrix.array[10] = -2 / (zFar - zNear);
    projectionMatrix.array[12] = (-right - left) / (right - left);
    projectionMatrix.array[13] = (-top - bottom) / (top - bottom);
    projectionMatrix.array[14] = (-zFar - zNear) / (zFar - zNear);
  }
}

Matrix4f Camera::getProjectionMatrix() {
  return projectionMatrix;
}

void Camera::setFovy(float fovy) {
  this->fovy = fovy;
  recalculate();
}

void Camera::setAspect(float aspect) {
  this->aspect = aspect;
  recalculate();
}

void Camera::setZNear(float zNear) {
  this->zNear = zNear;
  recalculate();
}

void Camera::setZFar(float zFar) {
  this->zFar = zFar;
  recalculate();
}

void Camera::setPerspective() {
  this->perspective = true;
  recalculate();
}

void Camera::setOrthographic() {
  this->perspective = false;
  recalculate();
}

void Camera::setLeft(float left) {
  this->left = left;
  recalculate();
}

void Camera::setRight(float right) {
  this->right = right;
  recalculate();
}

void Camera::setTop(float top) {
  this->top = top;
  recalculate();
}

void Camera::setBottom(float bottom) {
  this->bottom = bottom;
  recalculate();
}

} /* namespace glPortal */
