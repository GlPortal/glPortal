/* ---------------------------------------------------------------------------
** This software is in the public domain, furnished "as is", without technical
** support, and with no warranty, express or implied, as to its usefulness for
** any purpose.
**
** Vector3f.hpp
** Declares a vector consisting of 3 float values and its helper functions
**
** Author: Nim
** -------------------------------------------------------------------------*/

#pragma once
#ifndef VECTOR3F_HPP
#define VECTOR3F_HPP

#include <string>

class btVector3;

namespace glPortal {

class Vector3f {
public:
  union {
    float x, r, s, pitch;
  };
  union {
    float y, g, t, yaw;
  };
  union {
    float z, b, u, roll;
  };

  static const Vector3f ZERO;
  static const Vector3f FORWARD;
  static const Vector3f UP;

  /* Core */
  Vector3f();
  Vector3f(float x, float y, float z);
  void set(float x, float y, float z);
  void set(const Vector3f& v);
  Vector3f& normalise();

  float length() const;
  std::string str() const;

  /* Operator overloads */
  bool operator==(const Vector3f& v) const;
  bool operator!=(const Vector3f& v) const;
  Vector3f& operator+=(const Vector3f& v);
  Vector3f& operator-=(const Vector3f& v);
  Vector3f& operator*=(const Vector3f& v);
  Vector3f& operator*=(float scale);
  Vector3f& operator/=(const Vector3f& v);
  Vector3f operator+(const Vector3f& v) const;
  Vector3f operator-(const Vector3f& v) const;
  Vector3f operator-() const;
  Vector3f operator*(const Vector3f& v) const;
  Vector3f operator*(float scale) const;
  Vector3f operator/(float divisor) const;

  /* Bullet interop */
  Vector3f(const btVector3&);
  operator btVector3() const;
  Vector3f& operator=(const btVector3&);
};

/* Utility functions */
float dot(const Vector3f& v1, const Vector3f& v2);
Vector3f cross(const Vector3f& v1, const Vector3f& v2);
Vector3f negate(const Vector3f& v);
Vector3f normalise(const Vector3f& v);

} /* namespace glPortal */

#endif /* VECTOR3F_HPP */
