#include "Vector2f.hpp"

#include <math.h>
#include <stdio.h>

namespace glPortal {

Vector2f::Vector2f(float x, float y) {
  this->x = x;
  this->y = y;
}

void Vector2f::set(float x, float y) {
  this->x = x;
  this->y = y;
}

Vector2f* Vector2f::add(Vector2f v) {
  this->x += v.x;
  this->y += v.y;
  return this;
}

Vector2f* Vector2f::subtract(Vector2f v) {
  this->x -= v.x;
  this->y -= v.y;
  return this;
}

float Vector2f::length() {
  return sqrt(x * x + y * y);
}

Vector2f* Vector2f::normalize() {
  float l = length();
  this->x /= l;
  this->y /= l;
  return this;
}

void Vector2f::print() {
  printf("Vector2f<%f, %f>\n", x, y);
}

Vector2f Vector2f::add(Vector2f v1, Vector2f v2) {
  Vector2f v;
  v.x = v1.x + v2.x;
  v.y = v1.y + v2.y;
  return v;
}

Vector2f Vector2f::sub(Vector2f v1, Vector2f v2) {
  Vector2f v;
  v.x = v1.x - v2.x;
  v.y = v1.y - v2.y;
  return v;
}

Vector2f Vector2f::negate(Vector2f v) {
  Vector2f vn(-v.x, -v.y);
  return vn;
}

} /* namespace glPortal */
