#ifndef VECTOR4F_HPP
#define VECTOR4F_HPP

#include <cmath>

class btVector4;
class btQuaternion;

namespace glPortal {

struct Vector2f;
struct Vector3f;
class Matrix4f;

struct Vector4f {
  union {
    float x, r, s;
  };
  union {
    float y, g, t;
  };
  union {
    float z, b, u;
  };
  union {
    float w, a, v, d;
  };

  Vector4f()
    : Vector4f(0, 0, 0, 0) {}
  Vector4f(float x, float y, float z, float w)
    : x(x), y(y), z(z), w(w) {}
  Vector4f(float v)
    : x(v), y(v), z(v), d(v) {}
  Vector4f(const Vector3f&, float w);
  Vector4f(const Vector2f&, float z, float w);

  void set(float x, float y, float z, float w) {
    this->x = x;
    this->y = y;
    this->z = z;
    this->w = w;
  }

  Vector4f operator-() {
    return Vector4f(-x, -y, -z, -w);
  }

  Vector4f operator*(const Vector4f &v) {
    return Vector4f(x*v.x, y*v.y, z*v.z, w*v.w);
  }
  Vector4f& operator*=(const Vector4f &v) {
    x *= v.x; y *= v.y; z *= v.z; w *= v.w;
    return *this;
  }

  Vector4f operator*(float v) {
    return Vector4f(x*v, y*v, z*v, w*v);
  }
  Vector4f& operator*=(float v) {
    x *= v; y *= v; z *= v; w *= v;
    return *this;
  }

  Vector4f operator/(const Vector4f &v) {
    return Vector4f(x/v.x, y/v.y, z/v.z, w/v.w);
  }
  Vector4f& operator/=(const Vector4f &v) {
    x /= v.x; y /= v.y; z /= v.z; w /= v.w;
    return *this;
  }

  Vector4f operator/(float v) {
    return Vector4f(x/v, y/v, z/v, w/v);
  }
  Vector4f& operator/=(float v) {
    x /= v; y /= v; z /= v; w /= v;
    return *this;
  }

  Vector4f operator+(const Vector4f &v) {
    return Vector4f(x+v.x, y+v.y, z+v.z, w+v.w);
  }
  Vector4f& operator+=(const Vector4f &v) {
    x += v.x; y += v.y; z += v.z; w += v.w;
    return *this;
  }

  Vector4f operator-(const Vector4f &v) {
    return Vector4f(x-v.x, y-v.y, z-v.z, w-v.w);
  }
  Vector4f& operator-=(const Vector4f &v) {
    x -= v.x; y -= v.y; z -= v.z; w -= v.w;
    return *this;
  }

  operator btVector4() const;
  Vector4f& operator=(const btVector4&);

  operator btQuaternion() const;
  Vector4f& operator=(const btQuaternion&);
};

struct Quaternion : public Vector4f {
  enum EulerOrder {
    XYZ,
    XZY,
    YXZ,
    YZX,
    ZXY,
    ZYX
  };
  using Vector4f::Vector4f;
  Quaternion()
    : Vector4f(0, 0, 0, 1) {}

  Quaternion operator*(const Quaternion&);
  Quaternion& operator*=(const Quaternion&);

  // Quaternion multiplication isn't commutative, and so isn't division
  Quaternion operator/(const Quaternion&) = delete;
  Quaternion& operator/=(const Quaternion&) = delete;

  void fromAxAngle(float x, float y, float z, float r);
  void fromAxAngle(const Vector3f &a, float r);
  void fromAxAngle(const Vector4f &a);

  void setFromEuler(float pitch, float yaw, float roll);
  void setFromEuler(const Vector3f&);

  Vector4f toAxAngle() const;
  Matrix4f toMatrix() const;

  using Vector4f::operator=;
  using Vector4f::operator btVector4;
  using Vector4f::operator btQuaternion;
};

inline float length(const Vector4f &v) {
  return std::sqrt(v.x*v.x + v.y*v.y + v.z*v.z + v.d*v.d);
}

inline float dot(const Vector4f &v, const Vector4f &w) {
  return v.x*w.x + v.y*w.y + v.z*w.z + v.w*w.w;
}

Vector4f normalize(const Vector4f&);
Quaternion conjugate(const Quaternion&);

} /* namespace glPortal */

#endif /* VECTOR4F_HPP */
