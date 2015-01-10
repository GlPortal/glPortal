#include "Matrix4f.hpp"

#include "Math.hpp"
#include <math.h>
#include <stdio.h>

#include "Vector3f.hpp"

namespace glPortal {

Matrix4f::Matrix4f() {
  setIdentity();
}

Matrix4f::Matrix4f(float array[]) {
  for (int i = 0; i < 16; i++) {
    this->array[i] = array[i];
  }
}

void Matrix4f::setIdentity() {
  for (int i = 0; i < 16; i++) {
    if (i % 5 == 0) {
      array[i] = 1;
    } else {
      array[i] = 0;
    }
  }
}

void Matrix4f::translate(Vector3f v) {
  translate(v.x, v.y, v.z);
}

void Matrix4f::translate(float x, float y, float z) {
  array[12] += array[0] * x + array[4] * y + array[8] * z;
  array[13] += array[1] * x + array[5] * y + array[9] * z;
  array[14] += array[2] * x + array[6] * y + array[10] * z;
  array[15] += array[3] * x + array[7] * y + array[11] * z;
}

void Matrix4f::rotate(float angle, float x, float y, float z) {
  float c = cos(Math::toRadians(angle));
  float s = sin(Math::toRadians(angle));
  float ic = 1 - c;

  float f0 = array[0] * ((x * x * ic) + c) + array[4] * ((x * y * ic) + (z * s))
      + array[8] * ((x * z * ic) - (y * s));
  float f1 = array[1] * ((x * x * ic) + c) + array[5] * ((x * y * ic) + (z * s))
      + array[9] * ((x * z * ic) - (y * s));
  float f2 = array[2] * ((x * x * ic) + c) + array[6] * ((x * y * ic) + (z * s))
      + array[10] * ((x * z * ic) - (y * s));
  float f3 = array[3] * ((x * x * ic) + c) + array[7] * ((x * y * ic) + (z * s))
      + array[11] * ((x * z * ic) - (y * s));

  float f4 = array[0] * ((x * y * ic) - (z * s)) + array[4] * ((y * y * ic) + c)
      + array[8] * ((y * z * ic) + (x * s));
  float f5 = array[1] * ((x * y * ic) - (z * s)) + array[5] * ((y * y * ic) + c)
      + array[9] * ((y * z * ic) + (x * s));
  float f6 = array[2] * ((x * y * ic) - (z * s)) + array[6] * ((y * y * ic) + c)
      + array[10] * ((y * z * ic) + (x * s));
  float f7 = array[3] * ((x * y * ic) - (z * s)) + array[7] * ((y * y * ic) + c)
      + array[11] * ((y * z * ic) + (x * s));

  float f8 = array[0] * ((x * z * ic) + (y * s))
      + array[4] * ((y * z * ic) - (x * s)) + array[8] * ((z * z * ic) + c);
  float f9 = array[1] * ((x * z * ic) + (y * s))
      + array[5] * ((y * z * ic) - (x * s)) + array[9] * ((z * z * ic) + c);
  float f10 = array[2] * ((x * z * ic) + (y * s))
      + array[6] * ((y * z * ic) - (x * s)) + array[10] * ((z * z * ic) + c);
  float f11 = array[3] * ((x * z * ic) + (y * s))
      + array[7] * ((y * z * ic) - (x * s)) + array[11] * ((z * z * ic) + c);

  array[0] = f0;
  array[1] = f1;
  array[2] = f2;
  array[3] = f3;
  array[4] = f4;
  array[5] = f5;
  array[6] = f6;
  array[7] = f7;
  array[8] = f8;
  array[9] = f9;
  array[10] = f10;
  array[11] = f11;
}

void Matrix4f::rotate(Vector3f v) {
  rotate(v.y, 0, 1, 0);
  rotate(v.x, 1, 0, 0);
  //rotate(v.z, 0, 0, 1);
}

void Matrix4f::scale(Vector3f v) {
  scale(v.x, v.y, v.z);
}

void Matrix4f::scale(float x, float y, float z) {
  array[0] = array[0] * x;
  array[1] = array[1] * x;
  array[2] = array[2] * x;
  array[3] = array[3] * x;
  array[4] = array[4] * y;
  array[5] = array[5] * y;
  array[6] = array[6] * y;
  array[7] = array[7] * y;
  array[8] = array[8] * z;
  array[9] = array[9] * z;
  array[10] = array[10] * z;
  array[11] = array[11] * z;
}

Vector3f Matrix4f::transform(Vector3f v) {
  Vector3f dest;
  dest.x = array[0] * v.x + array[4] * v.y + array[8] * v.z;
  dest.y = array[1] * v.x + array[5] * v.y + array[9] * v.z;
  dest.z = array[2] * v.x + array[6] * v.y + array[10] * v.z;

  return dest;
}

void Matrix4f::print() {
  printf("%f %f %f %f\n", array[0], array[4], array[8], array[12]);
  printf("%f %f %f %f\n", array[1], array[5], array[9], array[13]);
  printf("%f %f %f %f\n", array[2], array[6], array[10], array[14]);
  printf("%f %f %f %f\n", array[3], array[7], array[11], array[15]);
}

float determinant(const Matrix4f& m) {
  float f =
      m.array[0] * (m.array[5] * m.array[10] - m.array[6] * m.array[9])
      + m.array[1] * (m.array[6] * m.array[8] - m.array[4] * m.array[10])
      + m.array[2] * (m.array[4] * m.array[9] - m.array[5] * m.array[8]);
  return f;
}

Matrix4f inverse(const Matrix4f& m) {
  Matrix4f dest;
  float det = determinant(m);

  if (det != 0) {
     /* do it the ordinary way
      *
      * inv(A) = 1/det(A) * adj(T), where adj(T) = transpose(Conjugate Matrix)
      *
      * m00 m01 m02
      * m10 m11 m12
      * m20 m21 m22
      */
     float det_inv = 1.0f / det;

     // get the conjugate matrix
     float t00 = m.array[5] * m.array[10] - m.array[6] * m.array[9];
     float t01 = - m.array[4] * m.array[10] + m.array[6] * m.array[8];
     float t02 = m.array[4] * m.array[9] - m.array[5] * m.array[8];
     float t10 = - m.array[1] * m.array[10] + m.array[2] * m.array[9];
     float t11 = m.array[0] * m.array[10] - m.array[2] * m.array[8];
     float t12 = - m.array[0] * m.array[9] + m.array[1] * m.array[8];
     float t20 = m.array[1] * m.array[6] - m.array[2] * m.array[5];
     float t21 = -m.array[0] * m.array[6] + m.array[2] * m.array[4];
     float t22 = m.array[0] * m.array[5] - m.array[1] * m.array[4];

     dest.array[0] = t00 * det_inv;
     dest.array[5] = t11 * det_inv;
     dest.array[10] = t22 * det_inv;
     dest.array[1] = t10 * det_inv;
     dest.array[4] = t01 * det_inv;
     dest.array[8] = t02 * det_inv;
     dest.array[2] = t20 * det_inv;
     dest.array[6] = t21 * det_inv;
     dest.array[9] = t12 * det_inv;
  }
  return dest;
}

} /* namespace glPortal */
