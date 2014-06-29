#include "Vector3f.hpp"

#include <math.h>
#include <stdio.h>

namespace glPortal {

Vector3f::Vector3f(float x, float y, float z) {
  this->x = x;
  this->y = y;
  this->z = z;
}

void Vector3f::set(float x, float y, float z) {
  this->x = x;
  this->y = y;
  this->z = z;
}

void Vector3f::set(Vector3f v) {
  this->x = v.x;
  this->y = v.y;
  this->z = v.z;
}

Vector3f* Vector3f::add(Vector3f v) {
  this->x += v.x;
  this->y += v.y;
  this->z += v.z;
  return this;
}

Vector3f* Vector3f::subtract(Vector3f v) {
  this->x -= v.x;
  this->y -= v.y;
  this->z -= v.z;
  return this;
}

float Vector3f::length() {
  return sqrt(x * x + y * y + z * z);
}

Vector3f* Vector3f::normalize() {
  float l = length();
  this->x /= l;
  this->y /= l;
  this->z /= l;
  return this;
}

void Vector3f::print() {
  printf("Vector3f<%f, %f, %f>\n", x, y, z);
}

Vector3f Vector3f::add(Vector3f v1, Vector3f v2) {
  Vector3f v;
  v.x = v1.x + v2.x;
  v.y = v1.y + v2.y;
  v.z = v1.z + v2.z;
  return v;
}

Vector3f Vector3f::sub(Vector3f v1, Vector3f v2) {
  Vector3f v;
  v.x = v1.x - v2.x;
  v.y = v1.y - v2.y;
  v.z = v1.z - v2.z;
  return v;
}

Vector3f Vector3f::scale(Vector3f v, float scale) {
  Vector3f vc;
  vc.x = v.x * scale;
  vc.y = v.y * scale;
  vc.z = v.z * scale;
  return vc;
}

Vector3f Vector3f::negate(Vector3f v) {
  Vector3f vn(-v.x, -v.y, -v.z);
  return vn;
}

} /* namespace glPortal */
