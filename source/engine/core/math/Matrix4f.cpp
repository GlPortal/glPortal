/* ---------------------------------------------------------------------------
** This software is in the public domain, furnished "as is", without technical
** support, and with no warranty, express or implied, as to its usefulness for
** any purpose.
**
** Matrix4f.cpp
** Implements a 4x4 matrix consisting of 16 float values and its helper functions
**
** Author: Nim
** -------------------------------------------------------------------------*/

#include "Matrix4f.hpp"

#include <cmath>
#include <cstring>
#include <sstream>

#include "Matrix3f.hpp"
#include "Vector3f.hpp"
#include "Quaternion.hpp"
#include "Math.hpp"

namespace glPortal {

static float Identity4[4*4] = {
  1.f, 0.f, 0.f, 0.f,
  0.f, 1.f, 0.f, 0.f,
  0.f, 0.f, 1.f, 0.f,
  0.f, 0.f, 0.f, 1.f
};

/* Core */
Matrix4f::Matrix4f() {
  setIdentity();
}

Matrix4f::Matrix4f(const Vector3f &t, const Quaternion &q) {
  setIdentity();
  translate(t);
  rotate(q);
}

void Matrix4f::setIdentity() {
  std::memcpy(a, Identity4, sizeof(Identity4));
}

void Matrix4f::translate(const Vector3f &v) {
  a[12] += a[0] * v.x + a[4] * v.y + a[8] * v.z;
  a[13] += a[1] * v.x + a[5] * v.y + a[9] * v.z;
  a[14] += a[2] * v.x + a[6] * v.y + a[10] * v.z;
  a[15] += a[3] * v.x + a[7] * v.y + a[11] * v.z;
}

void Matrix4f::rotate(float angle, float x, float y, float z) {
  float c = cos(angle);
  float s = sin(angle);
  float ic = 1 - c;

  float f0 =
    a[0] * ((x * x * ic) + c) + a[4] * ((x * y * ic) + (z * s)) + a[8] * ((x * z * ic) - (y * s));
  float f1 =
    a[1] * ((x * x * ic) + c) + a[5] * ((x * y * ic) + (z * s)) + a[9] * ((x * z * ic) - (y * s));
  float f2 =
    a[2] * ((x * x * ic) + c) + a[6] * ((x * y * ic) + (z * s)) + a[10] * ((x * z * ic) - (y * s));
  float f3 =
    a[3] * ((x * x * ic) + c) + a[7] * ((x * y * ic) + (z * s)) + a[11] * ((x * z * ic) - (y * s));

  float f4 =
    a[0] * ((x * y * ic) - (z * s)) + a[4] * ((y * y * ic) + c) + a[8] * ((y * z * ic) + (x * s));
  float f5 =
    a[1] * ((x * y * ic) - (z * s)) + a[5] * ((y * y * ic) + c) + a[9] * ((y * z * ic) + (x * s));
  float f6 =
    a[2] * ((x * y * ic) - (z * s)) + a[6] * ((y * y * ic) + c) + a[10] * ((y * z * ic) + (x * s));
  float f7 =
    a[3] * ((x * y * ic) - (z * s)) + a[7] * ((y * y * ic) + c) + a[11] * ((y * z * ic) + (x * s));

  float f8 =
    a[0] * ((x * z * ic) + (y * s)) + a[4] * ((y * z * ic) - (x * s)) + a[8] * ((z * z * ic) + c);
  float f9 =
    a[1] * ((x * z * ic) + (y * s)) + a[5] * ((y * z * ic) - (x * s)) + a[9] * ((z * z * ic) + c);
  float f10 =
    a[2] * ((x * z * ic) + (y * s)) + a[6] * ((y * z * ic) - (x * s)) + a[10] * ((z * z * ic) + c);
  float f11 =
    a[3] * ((x * z * ic) + (y * s)) + a[7] * ((y * z * ic) - (x * s)) + a[11] * ((z * z * ic) + c);

  a[0] = f0; a[1] = f1; a[2] = f2; a[3] = f3;
  a[4] = f4; a[5] = f5; a[6] = f6; a[7] = f7;
  a[8] = f8; a[9] = f9; a[10] = f10; a[11] = f11;
}

void Matrix4f::rotate(const Quaternion &quat) {
  *this = (*this * quat.toMatrix());
}

void Matrix4f::scale(float scale) {
  a[0] *= scale;
  a[1] *= scale;
  a[2] *= scale;
  a[3] *= scale;
  a[4] *= scale;
  a[5] *= scale;
  a[6] *= scale;
  a[7] *= scale;
  a[8] *= scale;
  a[9] *= scale;
  a[10] *= scale;
  a[11] *= scale;
  a[12] *= scale;
  a[13] *= scale;
  a[14] *= scale;
  a[15] *= scale;
}

void Matrix4f::scale(const Vector3f &scale) {
  a[0] *= scale.x;
  a[1] *= scale.x;
  a[2] *= scale.x;
  a[3] *= scale.x;
  a[4] *= scale.y;
  a[5] *= scale.y;
  a[6] *= scale.y;
  a[7] *= scale.y;
  a[8] *= scale.z;
  a[9] *= scale.z;
  a[10] *= scale.z;
  a[11] *= scale.z;
}

Vector3f Matrix4f::transform(const Vector3f &v) const {
  return Vector3f(
    a[0] * v.x + a[4] * v.y + a[8] * v.z,
    a[1] * v.x + a[5] * v.y + a[9] * v.z,
    a[2] * v.x + a[6] * v.y + a[10] * v.z
  );
}

float* Matrix4f::toArray() {
  return a;
}

std::string Matrix4f::str() const {
  std::stringstream ss;
  ss << "[" << a[0] << ", " << a[4] << ", " << a[8] << ", " << a[12] << "]\n";
  ss << "[" << a[1] << ", " << a[5] << ", " << a[9] << ", " << a[13] << "]\n";
  ss << "[" << a[2] << ", " << a[6] << ", " << a[10] << ", " << a[14] << "]\n";
  ss << "[" << a[3] << ", " << a[7] << ", " << a[11] << ", " << a[15] << "]\n";

  return ss.str();
}


Quaternion Matrix4f::getRotation() const {
  float trace = a[0] + a[5] + a[10], temp[4];
  if (trace > 0) {
    float s = std::sqrt(trace + 1.0);
    temp[3]=(s * 0.5);
    s = 0.5 / s;

    temp[0]=((a[7] - a[9]) * s);
    temp[1]=((a[8] - a[2]) * s);
    temp[2]=((a[1] - a[4]) * s);
  } else {
    int i = a[0] < a[5] ?
      (a[5] < a[10] ? 2 : 1) :
      (a[0] < a[10] ? 2 : 0);
    int j = (i + 1) % 3;
    int k = (i + 2) % 3;

    float s = std::sqrt(a[i+4*i] - a[j+4*j] - a[k+4*k] + 1.0);
    temp[i] = s * 0.5;
    s = 0.5 / s;

    temp[3] = (a[k+4*j] - a[j+4*k]) * s;
    temp[j] = (a[j+4*i] + a[i+4*j]) * s;
    temp[k] = (a[k+4*i] + a[i+4*k]) * s;
  }
  return Quaternion(temp[0], temp[1], temp[2], temp[3]);
}

Vector3f Matrix4f::getPosition() const {
  return Vector3f(a[12], a[13], a[14]);
}

/* Operator overloads */
bool Matrix4f::operator==(const Matrix4f &m) const {
  return std::memcmp(a, m.a, sizeof(a)) == 0;
}

bool Matrix4f::operator!=(const Matrix4f &m) const {
  return std::memcmp(a, m.a, sizeof(a)) != 0;
}

Matrix4f Matrix4f::operator*(const Matrix4f &m) const {
  Matrix4f d;
  d[0] = a[0] * m[0] + a[4] * m[1] + a[8] * m[2] + a[12] * m[3];
  d[1] = a[1] * m[0] + a[5] * m[1] + a[9] * m[2] + a[13] * m[3];
  d[2] = a[2] * m[0] + a[6] * m[1] + a[10] * m[2] + a[14] * m[3];
  d[3] = a[3] * m[0] + a[7] * m[1] + a[11] * m[2] + a[15] * m[3];

  d[4] = a[0] * m[4] + a[4] * m[5] + a[8] * m[6] + a[12] * m[7];
  d[5] = a[1] * m[4] + a[5] * m[5] + a[9] * m[6] + a[13] * m[7];
  d[6] = a[2] * m[4] + a[6] * m[5] + a[10] * m[6] + a[14] * m[7];
  d[7] = a[3] * m[4] + a[7] * m[5] + a[11] * m[6] + a[15] * m[7];

  d[8] = a[0] * m[8] + a[4] * m[9] + a[8] * m[10] + a[12] * m[11];
  d[9] = a[1] * m[8] + a[5] * m[9] + a[9] * m[10] + a[13] * m[11];
  d[10] = a[2] * m[8] + a[6] * m[9] + a[10] * m[10] + a[14] * m[11];
  d[11] = a[3] * m[8] + a[7] * m[9] + a[11] * m[10] + a[15] * m[11];

  d[12] = a[0] * m[12] + a[4] * m[13] + a[8] * m[14] + a[12] * m[15];
  d[13] = a[1] * m[12] + a[5] * m[13] + a[9] * m[14] + a[13] * m[15];
  d[14] = a[2] * m[12] + a[6] * m[13] + a[10] * m[14] + a[14] * m[15];
  d[15] = a[3] * m[12] + a[7] * m[13] + a[11] * m[14] + a[15] * m[15];

  return d;
}

Vector4f Matrix4f::operator*(const Vector4f &v) const {
  Vector4f dest;
  dest.x = a[0] * v.x + a[4] * v.y + a[8] * v.z + a[12] * v.w;
  dest.y = a[1] * v.x + a[5] * v.y + a[9] * v.z + a[13] * v.w;
  dest.z = a[2] * v.x + a[6] * v.y + a[10] * v.z + a[14] * v.w;
  dest.w = a[3] * v.x + a[7] * v.y + a[11] * v.z + a[15] * v.w;
  return dest;
}

Vector3f Matrix4f::operator*(const Vector3f &v) const {
  Vector3f dest;
  dest.x = a[0] * v.x + a[4] * v.y + a[8] * v.z;
  dest.y = a[1] * v.x + a[5] * v.y + a[9] * v.z;
  dest.z = a[2] * v.x + a[6] * v.y + a[10] * v.z;
  return dest;
}


/* Utility functions */
Matrix4f transpose(const Matrix4f &m) {
  Matrix4f d;
  d[0] = m[0];  d[4] = m[1];  d[8] = m[2];  d[12] = m[3];
  d[1] = m[4];  d[5] = m[5];  d[9] = m[6];  d[13] = m[7];
  d[2] = m[8];  d[6] = m[9];  d[10] = m[10];  d[14] = m[11];
  d[3] = m[12]; d[7] = m[13]; d[11] = m[14];  d[15] = m[15];
  return d;
}

float determinant(const Matrix4f &m) {
  float det =
    m[0] * m[5] * m[10] * m[15] + m[0] * m[9] * m[14] * m[7] +
    m[0] * m[13] * m[6] * m[11] + m[4] * m[1] * m[14] * m[11] +
    m[4] * m[9] * m[2] * m[15] + m[4] * m[13] * m[10] * m[3] +
    m[8] * m[1] * m[6] * m[15] + m[8] * m[5] * m[14] * m[3] +
    m[8] * m[13] * m[2] * m[7] + m[12] * m[1] * m[10] * m[7] +
    m[12] * m[5] * m[2] * m[11] + m[12] * m[9] * m[6] * m[3] -
    m[0] * m[5] * m[14] * m[11] - m[0] * m[9] * m[6] * m[15] -
    m[0] * m[13] * m[10] * m[7] - m[4] * m[1] * m[10] * m[15] -
    m[4] * m[9] * m[14] * m[3] - m[4] * m[13] * m[2] * m[11] -
    m[8] * m[1] * m[14] * m[7] - m[8] * m[5] * m[2] * m[15] -
    m[8] * m[13] * m[6] * m[3] - m[12] * m[1] * m[6] * m[11] -
    m[12] * m[5] * m[10] * m[3] - m[12] * m[9] * m[2] * m[7];

  return det;
}

Matrix4f inverse(const Matrix4f &m) {
  float det = determinant(m);

  Matrix4f d;
  d[0] = m[5] * m[10] * m[15] + m[9] * m[14] * m[7] + m[13] * m[6] * m[11]
       - m[5] * m[14] * m[11] - m[9] * m[6] * m[15] - m[13] * m[10] * m[7];
  d[1] = m[1] * m[14] * m[11] + m[9] * m[2] * m[15] + m[13] * m[10] * m[3]
       - m[1] * m[10] * m[15] - m[9] * m[14] * m[3] - m[13] * m[2] * m[11];
  d[2] = m[1] * m[6] * m[15] + m[5] * m[14] * m[3] + m[13] * m[2] * m[7]
       - m[1] * m[14] * m[7] - m[5] * m[2] * m[15] - m[13] * m[6] * m[3];
  d[3] = m[1] * m[10] * m[7] + m[5] * m[2] * m[11] + m[9] * m[6] * m[3]
       - m[1] * m[6] * m[11] - m[5] * m[10] * m[3] - m[9] * m[2] * m[7];
  d[4] = m[4] * m[14] * m[11] + m[8] * m[6] * m[15] + m[12] * m[10] * m[7]
       - m[4] * m[10] * m[15] - m[8] * m[14] * m[7] - m[12] * m[6] * m[11];
  d[5] = m[0] * m[10] * m[15] + m[8] * m[14] * m[3] + m[12] * m[2] * m[11]
       - m[0] * m[14] * m[11] - m[8] * m[2] * m[15] - m[12] * m[10] * m[3];
  d[6] = m[0] * m[14] * m[7] + m[4] * m[2] * m[15] + m[12] * m[6] * m[3]
       - m[0] * m[6] * m[15] - m[4] * m[14] * m[3] - m[12] * m[2] * m[7];
  d[7] = m[0] * m[6] * m[11] + m[4] * m[10] * m[3] + m[8] * m[2] * m[7]
       - m[0] * m[10] * m[7] - m[4] * m[2] * m[11] - m[8] * m[6] * m[3];
  d[8] = m[4] * m[9] * m[15] + m[8] * m[13] * m[7] + m[12] * m[5] * m[11]
       - m[4] * m[13] * m[11] - m[8] * m[5] * m[15] - m[12] * m[9] * m[7];
  d[9] = m[0] * m[13] * m[11] + m[8] * m[1] * m[15] + m[12] * m[9] * m[3]
        - m[0] * m[9] * m[15] - m[8] * m[13] * m[3] - m[12] * m[1] * m[11];
  d[10] = m[0] * m[5] * m[15] + m[4] * m[13] * m[3] + m[12] * m[1] * m[7]
        - m[0] * m[13] * m[7] - m[4] * m[1] * m[15] - m[12] * m[5] * m[3];
  d[11] = m[0] * m[9] * m[7] + m[4] * m[1] * m[11] + m[8] * m[5] * m[3]
        - m[0] * m[5] * m[11] - m[4] * m[9] * m[3] - m[8] * m[1] * m[7];
  d[12] = m[4] * m[13] * m[10] + m[8] * m[5] * m[14] + m[12] * m[9] * m[6]
        - m[4] * m[9] * m[14] - m[8] * m[13] * m[6] - m[12] * m[5] * m[10];
  d[13] = m[0] * m[9] * m[14] + m[8] * m[13] * m[2] + m[12] * m[1] * m[10]
        - m[0] * m[13] * m[10] - m[8] * m[1] * m[14] - m[12] * m[9] * m[2];
  d[14] = m[0] * m[13] * m[6] + m[4] * m[1] * m[14] + m[12] * m[5] * m[2]
        - m[0] * m[5] * m[14] - m[4] * m[13] * m[2] - m[12] * m[1] * m[6];
  d[15] = m[0] * m[5] * m[10] + m[4] * m[9] * m[2] + m[8] * m[1] * m[6]
        - m[0] * m[9] * m[6] - m[4] * m[1] * m[10] - m[8] * m[5] * m[2];

  d.scale(1 / det);

  return d;
}

Matrix3f toMatrix3f(const Matrix4f &m) {
  Matrix3f d;
  d[0] = m[0];  d[3] = m[4];  d[6] = m[8];
  d[1] = m[1];  d[4] = m[5];  d[7] = m[9];
  d[2] = m[2];  d[5] = m[6];  d[8] = m[10];
  return d;
}

} /* namespace glPortal */
