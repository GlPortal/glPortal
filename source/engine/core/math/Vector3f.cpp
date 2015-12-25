/* ---------------------------------------------------------------------------
** This software is in the public domain, furnished "as is", without technical
** support, and with no warranty, express or implied, as to its usefulness for
** any purpose.
**
** Vector3f.cpp
** Implements a vector consisting of 3 float values and its helper functions
**
** Author: Nim
** -------------------------------------------------------------------------*/

#include "Vector3f.hpp"
#include <cmath>
#include <sstream>
#include <bullet/LinearMath/btVector3.h>

namespace glPortal {

const Vector3f Vector3f::ZERO = Vector3f(0, 0, 0);
const Vector3f Vector3f::FORWARD = Vector3f(0, 0, -1);
const Vector3f Vector3f::UP = Vector3f(0, 1, 0);

/* Core */
void Vector3f::set(float x, float y, float z) {
  this->x = x;
  this->y = y;
  this->z = z;
}

void Vector3f::set(const Vector3f& v) {
  x = v.x;
  y = v.y;
  z = v.z;
}

float Vector3f::length() const {
  return sqrt(x * x + y * y + z * z);
}

std::string Vector3f::str() const {
  std::stringstream ss;
  ss << "(" << x << ", " << y << ", " << z << ")";
  return ss.str();
}

/* Operator overloads */
bool Vector3f::operator==(const Vector3f& v) const {
  return x == v.x && y == v.y && z == v.z;
}

bool Vector3f::operator!=(const Vector3f& v) const {
  return x != v.x || y != v.y || z != v.z;
}

bool Vector3f::fuzzyEqual(const Vector3f &v, float threshold) const {
  return (x > v.x - threshold and x < v.x + threshold) and
         (y > v.y - threshold and y < v.y + threshold) and
         (z > v.z - threshold and z < v.z + threshold);
}

/* Bullet interop */

Vector3f::Vector3f(const btVector3 &v) :
  Vector3f(v.x(), v.y(), v.z()) {
}

Vector3f::operator btVector3() const {
  return btVector3(x, y, z);
}

Vector3f& Vector3f::operator=(const btVector3 &v) {
  x = v.x(); y = v.y(); z = v.z();
  return *this;
}

/* Utility functions */
Vector3f cross(const Vector3f& v1, const Vector3f& v2) {
  Vector3f v;
  v.x = v1.y * v2.z - v1.z * v2.y;
  v.y = v2.x * v1.z - v2.z * v1.x;
  v.z = v1.x * v2.y - v1.y * v2.x;
  return v;
}

} /* namespace glPortal */
