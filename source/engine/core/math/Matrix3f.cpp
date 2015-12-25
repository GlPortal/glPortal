/* ---------------------------------------------------------------------------
** This software is in the public domain, furnished "as is", without technical
** support, and with no warranty, express or implied, as to its usefulness for
** any purpose.
**
** Matrix3f.cpp
** Implements a 3x3 matrix consisting of 9 float values and its helper functions
**
** Author: Nim
** -------------------------------------------------------------------------*/

#include "Matrix3f.hpp"

#include <cmath>
#include <cstring>
#include <sstream>

#include "Matrix4f.hpp"
#include "Vector3f.hpp"
#include "Vector2f.hpp"
#include "Math.hpp"

namespace glPortal {

static float Identity3[3*3] = {
  1.f, 0.f, 0.f,
  0.f, 1.f, 0.f,
  0.f, 0.f, 1.f
};

/* Core */
Matrix3f::Matrix3f() {
  setIdentity();
}

void Matrix3f::setIdentity() {
  std::memcpy(a, Identity3, sizeof(Identity3));
}

void Matrix3f::translate(const Vector2f& v) {
  a[6] += a[0] * v.x + a[3] * v.y;
  a[7] += a[1] * v.x + a[4] * v.y;
  a[8] += a[2] * v.x + a[5] * v.y;
}

void Matrix3f::rotate(float angle) {
  float c = cos(angle);
  float s = sin(angle);

  a[0] = c;
  a[1] = s;
  a[3] = -s;
  a[4] = c;
}

void Matrix3f::scale(float scale) {
  a[0] *= scale;
  a[1] *= scale;
  a[2] *= scale;
  a[3] *= scale;
  a[4] *= scale;
  a[5] *= scale;
  a[6] *= scale;
  a[7] *= scale;
  a[8] *= scale;
}

void Matrix3f::scale(const Vector2f& scale) {
  a[0] *= scale.x;
  a[1] *= scale.x;
  a[2] *= scale.x;
  a[3] *= scale.y;
  a[4] *= scale.y;
  a[5] *= scale.y;
}

Vector3f Matrix3f::transform(const Vector3f& v) const {
  Vector3f dest;
  dest.x = a[0] * v.x + a[3] * v.y + a[6] * v.z;
  dest.y = a[1] * v.x + a[4] * v.y + a[7] * v.z;
  dest.z = a[2] * v.x + a[5] * v.y + a[8] * v.z;
  return dest;
}

float* Matrix3f::toArray() {
  return a;
}

std::string Matrix3f::str() const {
  std::stringstream ss;
  ss << "[" << a[0] << ", " << a[3] << ", " << a[6] << "]\n";
  ss << "[" << a[1] << ", " << a[4] << ", " << a[7] << "]\n";
  ss << "[" << a[2] << ", " << a[5] << ", " << a[8] << "]\n";

  return ss.str();
}


/* Operator overloads */
bool Matrix3f::operator==(const Matrix3f& m) const {
  return std::memcmp(a, m.a, sizeof(a)) == 0;
}

bool Matrix3f::operator!=(const Matrix3f& m) const {
  return std::memcmp(a, m.a, sizeof(a)) != 0;
}

Matrix3f Matrix3f::operator*(const Matrix3f& m) const {
  Matrix3f d;
  d[0] = a[0] * m[0] + a[3] * m[1] + a[6] * m[2];
  d[1] = a[1] * m[0] + a[4] * m[1] + a[7] * m[2];
  d[2] = a[2] * m[0] + a[5] * m[1] + a[8] * m[2];

  d[3] = a[0] * m[3] + a[3] * m[4] + a[6] * m[5];
  d[4] = a[1] * m[3] + a[4] * m[4] + a[7] * m[5];
  d[5] = a[2] * m[3] + a[5] * m[4] + a[8] * m[5];

  d[6] = a[0] * m[6] + a[3] * m[7] + a[6] * m[8];
  d[7] = a[1] * m[6] + a[4] * m[7] + a[7] * m[8];
  d[8] = a[2] * m[6] + a[5] * m[7] + a[8] * m[8];

  return d;
}

/* Utility functions */
Matrix3f transpose(const Matrix3f& m) {
  Matrix3f d;
  d[0] = m[0];
  d[3] = m[1];
  d[6] = m[2];
  d[1] = m[3];
  d[4] = m[4];
  d[7] = m[5];
  d[2] = m[6];
  d[5] = m[7];
  d[8] = m[8];
  return d;
}

float determinant(const Matrix3f& m) {
  float det =
      m[0] * m[4] * m[8] + m[1] * m[5] * m[6] + m[2] * m[3] * m[7] -
      m[0] * m[5] * m[7] - m[2] * m[4] * m[6] - m[1] * m[3] * m[8];

  return det;
}

Matrix3f inverse(const Matrix3f& m) {
  float det = determinant(m);

  Matrix3f d;
  d[0] = m[4] * m[8] - m[7] * m[5];
  d[1] = m[7] * m[2] - m[1] * m[8];
  d[2] = m[1] * m[5] - m[4] * m[2];
  d[3] = m[6] * m[5] - m[3] * m[8];
  d[4] = m[0] * m[8] - m[6] * m[2];
  d[5] = m[3] * m[2] - m[0] * m[5];
  d[6] = m[3] * m[7] - m[6] * m[4];
  d[7] = m[6] * m[1] - m[0] * m[7];
  d[8] = m[0] * m[4] - m[3] * m[1];

  d.scale(1 / det);

  return d;
}

Matrix4f toMatrix4f(const Matrix3f& m) {
  Matrix4f d;
  d[0] = m[0];
  d[4] = m[3];
  d[8] = m[6];
  d[12] = 0;
  d[1] = m[1];
  d[5] = m[4];
  d[9] = m[7];
  d[13] = 0;
  d[2] = m[2];
  d[6] = m[5];
  d[10] = m[8];
  d[14] = 0;
  d[3] = 0;
  d[7] = 0;
  d[11] = 0;
  d[15] = 1;
  return d;
}

} /* namespace glPortal */
