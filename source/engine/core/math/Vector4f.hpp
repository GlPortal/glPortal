#ifndef VECTOR4F_HPP
#define VECTOR4F_HPP

#include <cmath>
#include <string>

class btVector4;
class btQuaternion;

namespace glPortal {

struct Vector2f;
struct Vector3f;
class Matrix3f;
class Matrix4f;

/** \class Vector4f
 * @brief 4-dimensional `float`-based vector/point storage and manipulation struct
 */
struct Vector4f {
  union {
    float x, r, s, yaw, heading, azimuth, tetha;
  };
  union {
    float y, g, t, pitch, attitude, elevation, phi;
  };
  union {
    float z, b, u, roll, bank, tilt, psi;
  };
  union {
    float w, a, v, d;
  };

  constexpr Vector4f()
    : x(0), y(0), z(0), w(0) {}
  constexpr Vector4f(float x, float y, float z, float w)
    : x(x), y(y), z(z), w(w) {}
  constexpr Vector4f(float v)
    : x(v), y(v), z(v), d(v) {}
  constexpr Vector4f(const Vector3f&, float w);
  constexpr Vector4f(const Vector2f&, float z, float w);

  void set(float x, float y, float z, float w) {
    this->x = x;
    this->y = y;
    this->z = z;
    this->w = w;
  }

  Vector4f operator-() const {
    return Vector4f(-x, -y, -z, -w);
  }

  Vector4f operator*(const Vector4f &v) const {
    return Vector4f(x*v.x, y*v.y, z*v.z, w*v.w);
  }
  Vector4f& operator*=(const Vector4f &v) {
    x *= v.x; y *= v.y; z *= v.z; w *= v.w;
    return *this;
  }

  Vector4f operator*(float v) const {
    return Vector4f(x*v, y*v, z*v, w*v);
  }
  Vector4f& operator*=(float v) {
    x *= v; y *= v; z *= v; w *= v;
    return *this;
  }

  Vector4f operator/(const Vector4f &v) const {
    return Vector4f(x/v.x, y/v.y, z/v.z, w/v.w);
  }
  Vector4f& operator/=(const Vector4f &v) {
    x /= v.x; y /= v.y; z /= v.z; w /= v.w;
    return *this;
  }

  Vector4f operator/(float v) const {
    return Vector4f(x/v, y/v, z/v, w/v);
  }
  Vector4f& operator/=(float v) {
    x /= v; y /= v; z /= v; w /= v;
    return *this;
  }

  Vector4f operator+(const Vector4f &v) const {
    return Vector4f(x+v.x, y+v.y, z+v.z, w+v.w);
  }
  Vector4f& operator+=(const Vector4f &v) {
    x += v.x; y += v.y; z += v.z; w += v.w;
    return *this;
  }

  Vector4f operator-(const Vector4f &v) const {
    return Vector4f(x-v.x, y-v.y, z-v.z, w-v.w);
  }
  Vector4f& operator-=(const Vector4f &v) {
    x -= v.x; y -= v.y; z -= v.z; w -= v.w;
    return *this;
  }

  bool fuzzyEqual(const Vector4f&, float threshold = .01f) const;

  inline std::string toString() const {
    return (std::string("Vec4f{x=") + std::to_string(x) +
      ", y=" + std::to_string(y) +
      ", z=" + std::to_string(z) +
      ", w=" + std::to_string(w) + "}");
  }

  operator btVector4() const;
  Vector4f(const btVector4&);
  Vector4f& operator=(const btVector4&);

  operator btQuaternion() const;
  Vector4f(const btQuaternion&);
  Vector4f& operator=(const btQuaternion&);
};

struct Quaternion : public Vector4f {
  using Vector4f::Vector4f;
  constexpr Quaternion()
    : Vector4f(0, 0, 0, 1) {}
  Quaternion(const btQuaternion&);

  Quaternion operator*(const Quaternion&) const;
  Quaternion& operator*=(const Quaternion&);

  Vector3f operator*(const Vector3f&) const;

  // Quaternion multiplication isn't commutative, and so isn't division
  Quaternion operator/(const Quaternion&) const = delete;
  Quaternion& operator/=(const Quaternion&) = delete;


  Quaternion& fromAxAngle(float x, float y, float z, float r);
  Quaternion& fromAxAngle(const Vector3f &a, float r);
  Quaternion& fromAxAngle(const Vector4f &a);

  Vector4f toAxAngle() const;


  Quaternion& fromAero(float tetha, float phi, float psi);
  Quaternion& fromAero(const Vector3f&);

  Vector3f toAero() const;

  Matrix4f toMatrix() const;

  inline std::string toString() const {
    return (std::string("Quat{x=") + std::to_string(x) +
      ", y=" + std::to_string(y) +
      ", z=" + std::to_string(z) +
      ", w=" + std::to_string(w) + "}");
  }

  using Vector4f::operator=;
  using Vector4f::operator btVector4;
  using Vector4f::operator btQuaternion;
};

constexpr inline float length2(const Vector4f &v) {
  return v.x*v.x + v.y*v.y + v.z*v.z + v.d*v.d;
}

constexpr inline float length(const Vector4f &v) {
  return std::sqrt(v.x*v.x + v.y*v.y + v.z*v.z + v.d*v.d);
}

constexpr inline float dot(const Vector4f &v, const Vector4f &w) {
  return v.x*w.x + v.y*w.y + v.z*w.z + v.w*w.w;
}

inline Vector4f normalize(const Vector4f &v) {
  return v / length(v);
}

/*constexpr*/ inline Quaternion normalize(const Quaternion &q) {
  float l = length(q);
  return Quaternion(q.x/l, q.y/l, q.z/l, q.w/l);
}

constexpr inline Quaternion conjugate(const Quaternion &q) {
  return Quaternion(-q.x, -q.y, -q.z, q.w);
}

} /* namespace glPortal */

#endif /* VECTOR4F_HPP */
