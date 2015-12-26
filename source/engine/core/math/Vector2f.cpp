/* ---------------------------------------------------------------------------
** This software is in the public domain, furnished "as is", without technical
** support, and with no warranty, express or implied, as to its usefulness for
** any purpose.
**
** Vector2f.cpp
** Implements a vector consisting of 2 float values and its helper functions
**
** Author: Nim
** -------------------------------------------------------------------------*/

#include "Vector2f.hpp"
#include <cmath>
#include <sstream>

namespace glPortal {

const Vector2f Vector2f::ZERO = Vector2f(0, 0);
const Vector2f Vector2f::UP = Vector2f(0, 1);

/* Core */
void Vector2f::set(float x, float y) {
  this->x = x;
  this->y = y;
}

float Vector2f::length() const {
  return sqrt(x * x + y * y);
}

std::string Vector2f::str() const {
  std::stringstream ss;
  ss << "(" << x << ", " << y << ")";
  return ss.str();
}

/* Operator overloads */
bool Vector2f::operator==(const Vector2f& v) const {
  return x == v.x && y == v.y;
}

bool Vector2f::operator!=(const Vector2f& v) const {
  return x != v.x || y != v.y;
}

Vector2f& Vector2f::operator+=(const Vector2f& v) {
  x += v.x;
  y += v.y;
  return *this;
}

Vector2f& Vector2f::operator-=(const Vector2f& v) {
  x -= v.x;
  y -= v.y;
  return *this;
}

Vector2f& Vector2f::operator*=(const Vector2f& v) {
  x *= v.x;
  y *= v.y;
  return *this;
}

Vector2f& Vector2f::operator*=(float scale) {
  x *= scale;
  y *= scale;
  return *this;
}

Vector2f& Vector2f::operator/=(const Vector2f& v) {
  x /= v.x;
  y /= v.y;
  return *this;
}

Vector2f Vector2f::operator+(const Vector2f& v) const {
  return Vector2f(x + v.x, y + v.y);
}

Vector2f Vector2f::operator-(const Vector2f& v) const {
  return Vector2f(x - v.x, y - v.y);
}

Vector2f Vector2f::operator-() const {
  return Vector2f(-x, -y);
}

Vector2f Vector2f::operator*(float scale) const {
  return Vector2f(x * scale, y * scale);
}

Vector2f Vector2f::operator/(float divisor) const {
  return Vector2f(x / divisor, y / divisor);
}

bool Vector2f::fuzzyEqual(const Vector2f &v, float threshold) const {
  return (x > v.x - threshold and x < v.x + threshold) and
         (y > v.y - threshold and y < v.y + threshold);
}

/* Utility functions */
float dot(const Vector2f& v1, const Vector2f& v2) {
  return v1.x * v2.x + v1.y * v2.y;
}

Vector2f negate(const Vector2f& v) {
  return Vector2f(-v.x, -v.y);
}

Vector2f normalize(const Vector2f& v) {
  float length = v.length();
  return Vector2f(v.x / length, v.y / length);
}

} /* namespace glPortal */
