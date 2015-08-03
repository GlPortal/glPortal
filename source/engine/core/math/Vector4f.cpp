#include "Vector4f.hpp"

#include <bullet/LinearMath/btQuaternion.h>
#include <bullet/LinearMath/btVector3.h>
#include <engine/core/math/Vector2f.hpp>
#include <engine/core/math/Vector3f.hpp>
#include <engine/core/math/Matrix4f.hpp>

namespace glPortal {

Vector4f::Vector4f(const Vector3f &v, float w)
    : x(v.x), y(v.y), z(v.z), w(w) {}

Vector4f::Vector4f(const Vector2f &v, float z, float w)
  : x(v.x), y(v.y), z(z), w(w) {}

Vector4f normalize(const Vector4f &v) {
  float len = length(v);
  return Vector4f(v.x/len, v.y/len, v.z/len, v.w/len);
}

Vector4f::operator btVector4() const {
  return btVector4(x, y, z, w);
}

Vector4f& Vector4f::operator=(const btVector4 &v) {
  x = v.x(); y = v.y(); z = v.z(); w = v.w();
  return *this;
}

Vector4f::operator btQuaternion() const {
  return btQuaternion(x, y, z, w);
}

Vector4f& Vector4f::operator=(const btQuaternion &q) {
  x = q.x(); y = q.y(); z = q.z(); w = q.w();
  return *this;
}

// === Quaternion === //

Quaternion Quaternion::operator*(const Quaternion &q) {
  return Quaternion(
    w * q.x + x * q.w + y * q.z - z * q.y,
    w * q.y + y * q.w + z * q.x - x * q.z,
    w * q.z + z * q.w + x * q.y - y * q.x,
    w * q.w - x * q.x - y * q.y - z * q.z
  );
}

Quaternion& Quaternion::operator*=(const Quaternion &q) {
  x = w * q.x + x * q.w + y * q.z - z * q.y;
  y = w * q.y + y * q.w + z * q.x - x * q.z;
  z = w * q.z + z * q.w + x * q.y - y * q.x;
  w = w * q.w - x * q.x - y * q.y - z * q.z;
  return *this;
}

void Quaternion::fromAxAngle(const Vector3f &a, float r) {
  Vector3f na = normalise(a);
  register float sinR2 = std::sin(r / 2);
  this->x = na.x * sinR2;
  this->y = na.y * sinR2;
  this->z = na.z * sinR2;
  this->w = std::cos(r / 2);
}

void Quaternion::fromAxAngle(float x, float y, float z, float r) {
  fromAxAngle(Vector3f(x, y, z), r);
}

void Quaternion::fromAxAngle(const Vector4f &a) {
  fromAxAngle(a.x, a.y, a.z, a.w);
}

void Quaternion::fromEuler(float p, float y, float z) {
  float c1 = std::cos(p/2);
  float s1 = std::sin(p/2);
  float c2 = std::cos(y/2);
  float s2 = std::sin(y/2);
  float c3 = std::cos(r/2);
  float s3 = std::sin(r/2);
  float c1c2 = c1*c2;
  float s1s2 = s1*s2;
  this->x = c1c2*s3 + s1s2*c3;
  this->y = s1*c2*c3 + c1*s2*s3;
  this->z = c1*s2*c3 - s1*c2*s3;
  this->w = c1c2*c3 - s1s2*s3;
  // Normalise:
  float invlen = 1/length(*this);
  this->x *= invlen;
  this->y *= invlen;
  this->z *= invlen;
  this->w *= invlen;
}

void Quaternion::fromEuler(const Vector3f &e) {
  fromEuler(e.pitch, e.yaw, e.roll);
}

Vector4f Quaternion::toAxAngle() const {
  register float r = std::acos(w)*2, invSinR2 = 1/std::sin(r/2);
  return Vector4f(
    x * invSinR2,
    y * invSinR2,
    z * invSinR2,
    r
  );
}

Matrix4f Quaternion::toMatrix() const {
  Matrix4f m;
  m[0] = 1.0f - 2.0f*y*y - 2.0f*z*z;
  m[1] = 2.0f*x*y + 2.0f*z*w;
  m[2] = 2.0f*x*z - 2.0f*y*w;
  m[4] = 2.0f*x*y - 2.0f*z*w;
  m[5] = 1.0f - 2.0f*x*x - 2.0f*z*z;
  m[6] = 2.0f*y*z + 2.0f*x*w;
  m[8] = 2.0f*x*z + 2.0f*y*w;
  m[9] = 2.0f*y*z - 2.0f*x*w;
  m[10] = 1.0f - 2.0f*x*x - 2.0f*y*y;
  return m;
}

Quaternion conjugate(const Quaternion &q) {
  return Quaternion(-q.x, -q.y, -q.z, q.w);
}

} /* namespace glPortal */
