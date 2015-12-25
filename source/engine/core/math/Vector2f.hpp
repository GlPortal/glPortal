/* ---------------------------------------------------------------------------
** This software is in the public domain, furnished "as is", without technical
** support, and with no warranty, express or implied, as to its usefulness for
** any purpose.
**
** Vector2f.hpp
** Declares a vector consisting of 2 float values and its helper functions
**
** Author: Nim
** -------------------------------------------------------------------------*/

#pragma once
#ifndef VECTOR2F_HPP
#define VECTOR2F_HPP

#include <string>

namespace glPortal {

/** \class Vector2f
 * @brief 2-dimensional `float`-based vector/point storage and manipulation struct
 */
struct Vector2f {
  union {
    float x, r, s, u;
  };
  union {
    float y, g, t, v;
  };

  static const Vector2f ZERO, UP;

  /* Core */
  constexpr Vector2f()
    : x(0), y(0) {}
  constexpr Vector2f(float x, float y)
    : x(x), y(y) {}
  constexpr Vector2f(float v)
    : x(v), y(v) {}

  void set(float x, float y);

  float length() const;
  std::string str() const;

  /* Operator overloads */
  bool operator==(const Vector2f& v) const;
  bool operator!=(const Vector2f& v) const;
  Vector2f& operator+=(const Vector2f& v);
  Vector2f& operator-=(const Vector2f& v);
  Vector2f& operator*=(const Vector2f& v);
  Vector2f& operator*=(float scale);
  Vector2f& operator/=(const Vector2f& v);
  Vector2f operator+(const Vector2f& v) const;
  Vector2f operator-(const Vector2f& v) const;
  Vector2f operator-() const;
  Vector2f operator*(float scale) const;
  Vector2f operator/(float divisor) const;

  bool fuzzyEqual(const Vector2f&, float threshold = .02f) const;
};

/* Utility functions */
float dot(const Vector2f& v1, const Vector2f& v2);
Vector2f negate(const Vector2f& v);
Vector2f normalize(const Vector2f& v);

} /* namespace glPortal */

#endif /* VECTOR2F_HPP */
