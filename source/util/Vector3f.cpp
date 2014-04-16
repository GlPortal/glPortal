#include "Vector3f.hpp"
#include <math.h>

using namespace util;

Vector3f::Vector3f(float x, float y, float z) {
  this->x = x;
  this->y = y;
  this->z = z;
}

Vector3f *Vector3f::set(float x, float y, float z) {
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

float Vector3f::length() {
  return sqrt(x*x + y*y + z*z);
}

Vector3f *Vector3f::normalize() {
  float l = length();
  this->x /= l;
  this->y /= l;
  this->z /= l;
  return this;
}

