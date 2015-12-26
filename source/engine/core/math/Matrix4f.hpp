/* ---------------------------------------------------------------------------
** This software is in the public domain, furnished "as is", without technical
** support, and with no warranty, express or implied, as to its usefulness for
** any purpose.
**
** Matrix4f.hpp
** Declares a 4x4 matrix consisting of 16 float values and its helper functions
**
** Author: Nim
** -------------------------------------------------------------------------*/

#pragma once
#ifndef MATRIX4F_HPP
#define MATRIX4F_HPP

#include <string>

namespace glPortal {

class Matrix3f;
class Vector3f;
class Vector4f;
class Quaternion;

class Matrix4f {
public:
  /* Core */
  Matrix4f();
  Matrix4f(const Vector3f&, const Quaternion&);
  void setIdentity();
  void translate(const Vector3f &v);
  void rotate(float angle, float x, float y, float z);
  void rotate(const Quaternion &quat);
  void scale(float scale);
  void scale(const Vector3f &scale);
  Vector3f transform(const Vector3f &v) const;

  float* toArray();
  std::string str() const;

  Quaternion getRotation() const;
  Vector3f getPosition() const;

  /* Operator overloads */
  inline float operator[](int i) const {
    return a[i];
  }
  inline float& operator[](int i) {
    return a[i];
  }
  bool operator==(const Matrix4f&) const;
  bool operator!=(const Matrix4f&) const;
  Matrix4f operator*(const Matrix4f&) const;
  Vector4f operator*(const Vector4f&) const;
  Vector3f operator*(const Vector3f&) const;
private:
  float a[16];
};

/* Utility functions */
Matrix4f transpose(const Matrix4f& m);
float determinant(const Matrix4f& m);
Matrix4f inverse(const Matrix4f& m);
Matrix3f toMatrix3f(const Matrix4f& m);

} /* namespace glPortal */

#endif /* MATRIX4F_HPP */
