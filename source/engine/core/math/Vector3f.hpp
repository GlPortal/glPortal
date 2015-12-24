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
    float x, r, s, yaw, heading, azimuth, tetha;
  };
  union {
    float y, g, t, pitch, attitude, elevation, phi;
  };
  union {
    float z, b, u, roll, bank, tilt, psi;
  };

  static const Vector3f ZERO, FORWARD, UP;

  /* Core */
  constexpr Vector3f()
    : x(0), y(0), z(0) {}
  constexpr Vector3f(float x, float y, float z)
    : x(x), y(y), z(z) {}
  constexpr Vector3f(float v)
    : x(v), y(v), z(v) {}
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

  bool fuzzyEqual(const Vector3f&, float threshold = .02f) const;

  /* Bullet interop */
  Vector3f(const btVector3&);
  operator btVector3() const;
  Vector3f& operator=(const btVector3&);
};

/* Utility functions */
constexpr inline float dot(const Vector3f &v1, const Vector3f &v2) {
  return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}
Vector3f cross(const Vector3f &v1, const Vector3f &v2);
Vector3f normalize(const Vector3f &v);

} /* namespace glPortal */

#endif /* VECTOR3F_HPP */
