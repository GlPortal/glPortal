#include "Vector3f.hpp"

#include <math.h>
#include <sstream>

namespace glPortal {

Vector3f::Vector3f(float x, float y, float z): x(0), y(0), z(0) {
  this->set(x, y, z);
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
  x += v.x;
  y += v.y;
  z += v.z;
  return this;
}

Vector3f* Vector3f::sub(Vector3f v) {
  x -= v.x;
  y -= v.y;
  z -= v.z;
  return this;
}

Vector3f* Vector3f::scale(float scale) {
  x *= scale;
  y *= scale;
  z *= scale;
  return this;
}

Vector3f* Vector3f::normalise() {
  float l = length();
  x /= l;
  y /= l;
  z /= l;
  return this;
}

float Vector3f::length() {
  return sqrt(x * x + y * y + z * z);
}

const std::string Vector3f::str() const {
  std::stringstream ss;
  ss << "<x: " << x << " y: " << y << " z: " << z << ">";
  return ss.str();
}

Vector3f add(const Vector3f& v1, const Vector3f& v2) {
  Vector3f v;
  v.x = v1.x + v2.x;
  v.y = v1.y + v2.y;
  v.z = v1.z + v2.z;
  return v;
}

Vector3f sub(const Vector3f& v1, const Vector3f& v2) {
  Vector3f v;
  v.x = v1.x - v2.x;
  v.y = v1.y - v2.y;
  v.z = v1.z - v2.z;
  return v;
}

Vector3f scale(const Vector3f& v, float scale) {
  Vector3f vs;
  vs.x = v.x * scale;
  vs.y = v.y * scale;
  vs.z = v.z * scale;
  return vs;
}

float dot(const Vector3f& v1, const Vector3f& v2) {
  return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

Vector3f cross(const Vector3f& v1, const Vector3f& v2) {
  Vector3f v;
  v.x = v1.y * v2.z - v1.z * v2.y;
  v.y = v2.x * v1.z - v2.z * v1.x;
  v.z = v1.x * v2.y - v1.y * v2.x;
  return v;
}

Vector3f negate(const Vector3f& v) {
  Vector3f vn;
  vn.x = -v.x;
  vn.y = -v.y;
  vn.z = -v.z;
  return vn;
}

Vector3f operator+(const Vector3f& v1, const Vector3f& v2) {
  Vector3f v;
  v.x = v1.x + v2.x;
  v.y = v1.y + v2.y;
  v.z = v1.z + v2.z;
  return v;
}

Vector3f operator-(const Vector3f& v1, const Vector3f& v2) {
  Vector3f v;
  v.x = v1.x - v2.x;
  v.y = v1.y - v2.y;
  v.z = v1.z - v2.z;
  return v;
}

Vector3f operator*(const Vector3f& v, float scale) {
  Vector3f vs;
  vs.x = v.x * scale;
  vs.y = v.y * scale;
  vs.z = v.z * scale;
  return vs;
}

} /* namespace glPortal */
