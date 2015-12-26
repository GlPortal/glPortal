/* ---------------------------------------------------------------------------
** This software is in the public domain, furnished "as is", without technical
** support, and with no warranty, express or implied, as to its usefulness for
** any purpose.
**
** Matrix3f.hpp
** Declares a 3x3 matrix consisting of 9 float values and its helper functions
**
** Author: Nim
** -------------------------------------------------------------------------*/

#pragma once
#ifndef MATRIX3F_HPP
#define MATRIX3F_HPP

#include <string>

namespace glPortal {

class Matrix4f;
class Vector3f;
class Vector2f;

class Matrix3f {
public:
  /* Core */
  Matrix3f();
  void setIdentity();
  void translate(const Vector2f &v);
  void rotate(float angle);
  void scale(float scale);
  void scale(const Vector2f &scale);
  Vector3f transform(const Vector3f &v) const;

  float* toArray();
  std::string str() const;

  /* Operator overloads */
  inline float operator[](int i) const {
    return a[i];
  }
  inline float& operator[](int i) {
    return a[i];
  }
  bool operator==(const Matrix3f&) const;
  bool operator!=(const Matrix3f&) const;
  Matrix3f operator*(const Matrix3f&) const;
private:
  float a[9];
};

/* Utility functions */
Matrix3f transpose(const Matrix3f& m);
float determinant(const Matrix3f& m);
Matrix3f inverse(const Matrix3f& m);
Matrix4f toMatrix4f(const Matrix3f& m);

} /* namespace glPortal */

#endif /* MATRIX3F_HPP */
