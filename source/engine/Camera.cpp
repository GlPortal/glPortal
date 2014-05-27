#include "Camera.hpp"
#include <math.h>

#define PI 3.1415926535897932384626433832795028841971693993751058209749445923078164062862089

Camera::Camera(float fovy, float aspect, float zNear, float zFar) {
  this->fovy = fovy;
  this->aspect = aspect;
  this->zNear = zNear;
  this->zFar = zFar;
}

void Camera::getMatrix(float* matrix) {
  float fovyr = fovy * (PI/180);
  matrix[0] = (1/tan(fovyr/2))/aspect;
  matrix[5] = 1/tan(fovyr/2);
  matrix[10] = (zNear + zFar)/(zNear - zFar);
  matrix[11] = -1;
  matrix[14] = (2 * zNear * zFar) / (zNear - zFar);
  matrix[15] = 0;
}

