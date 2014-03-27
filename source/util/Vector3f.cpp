#include "Vector3f.hpp"
#include <math.h>

using namespace util;

Vector3f::Vector3f(float x, float y, float z) {
  this->x = x;
  this->y = y;
  this->z = z;
}

Vector3f *Vector3f::add(Vector3f v) {
  this->x += v.x;
  this->y += v.y;
  this->z += v.z;
  return this;
}

Vector3f *Vector3f::subtract(Vector3f v) {
  this->x -= v.x;
  this->y -= v.y;
  this->z -= v.z;
  return this;
}

Vector3f *Vector3f::normalize() {
  float length = sqrt(x*x + y*y + z*z);
  this->x /= length;
  this->y /= length;
  this->z /= length;
  return this;
}

