#include "Vector4f.hpp"

#include <bullet/LinearMath/btQuaternion.h>
#include <bullet/LinearMath/btVector3.h>
#include <engine/core/math/Vector2f.hpp>
#include <engine/core/math/Vector3f.hpp>
#include <engine/core/math/Matrix4f.hpp>

namespace glPortal {

constexpr Vector4f::Vector4f(const Vector3f &v, float w)
  : x(v.x), y(v.y), z(v.z), w(w) {}

constexpr Vector4f::Vector4f(const Vector2f &v, float z, float w)
  : x(v.x), y(v.y), z(z), w(w) {}

Vector4f::Vector4f(const btVector4 &v)
  : x(v.x()), y(v.y()), z(v.z()), w(v.w()) {}

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

bool Vector4f::fuzzyEqual(const Vector4f &v, float threshold) const {
  return (x > v.x - threshold and x < v.x + threshold) and
         (y > v.y - threshold and y < v.y + threshold) and
         (z > v.z - threshold and z < v.z + threshold) and
         (w > v.w - threshold and w < v.w + threshold);
}

Vector4f::operator btQuaternion() const {
  return btQuaternion(x, y, z, w);
}

Vector4f& Vector4f::operator=(const btQuaternion &q) {
  x = q.x(); y = q.y(); z = q.z(); w = q.w();
  return *this;
}

// === Quaternion === //

Quaternion::Quaternion(const btQuaternion &v)
  : Vector4f(v.x(), v.y(), v.z(), v.w()) {}

Quaternion Quaternion::operator*(const Quaternion &q) const {
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

Vector3f Quaternion::operator*(const Vector3f &v) const {
  // Pout = q * Pin * conj(q)
  Vector3f vn = normalize(v);
  Quaternion resQuat = *this * Quaternion(vn.x, vn.y, vn.z, 0) * conjugate(*this);
  return Vector3f(resQuat.x, resQuat.y, resQuat.z);
}


Quaternion& Quaternion::fromAxAngle(const Vector3f &a, float r) {
  Vector3f na = normalize(a);
  const float r2 = r / 2, sinR2 = std::sin(r2);
  this->x = na.x * sinR2;
  this->y = na.y * sinR2;
  this->z = na.z * sinR2;
  this->w = std::cos(r2);
  return *this;
}

Quaternion& Quaternion::fromAxAngle(float x, float y, float z, float r) {
  return fromAxAngle(Vector3f(x, y, z), r);
}

Quaternion& Quaternion::fromAxAngle(const Vector4f &a) {
  return fromAxAngle(a.x, a.y, a.z, a.w);
}

Vector4f Quaternion::toAxAngle() const {
  const float r = std::acos(w)*2, invSinR2 = 1/std::sin(r/2);
  return Vector4f(
    x * invSinR2,
    y * invSinR2,
    z * invSinR2,
    r
  );
}


Quaternion& Quaternion::fromAero(float tetha, float phi, float psi) {
  *this = Quaternion().fromAxAngle(0, 1, 0, tetha) *
          Quaternion().fromAxAngle(1, 0, 0, phi) *
          Quaternion().fromAxAngle(0, 0, 1, psi);
  return *this;
}

Quaternion& Quaternion::fromAero(const Vector3f &v) {
  fromAero(v.tetha, v.phi, v.psi);
  return *this;
}

Vector3f Quaternion::toAero() const {
  // FIXME!
  const double sqw = w*w, sqx = x*x, sqy = y*y, sqz = z*z;
  double unit = sqx + sqy + sqz + sqw; // if normalised is one, otherwise is correction factor
  double test = x*z + w*y;
  if (test > 0.499*unit) { // singularity at north pole
    return Vector3f(2 * std::atan2(x, w), M_PI/2, 0);
  }
  if (test < -0.499*unit) { // singularity at south pole
    return Vector3f(-2 * std::atan2(x, w), -M_PI/2, 0);
  }
  return Vector3f(
    std::atan2(-2*(x*z - w*y), sqw - sqx - sqy + sqz),
    std::asin(2*test/unit),
    std::atan2(-2*(x*y - w*z), sqw - sqx + sqy - sqz)
  );
}

#if 0
Quaternion& Quaternion::fromEulerZXY(const Vector3f &v) {
  fromEulerZXY(v.y, v.x, v.z);
  return *this;
}

Vector3f Quaternion::toEulerZXY() const {
  const Vector3f a = toAero();
  return Vector3f(a.y, a.x, a.z);
}
#endif

Quaternion& Quaternion::setFromEuler(float p, float y, float r) {
  float c1 = std::cos(y/2);
  float s1 = std::sin(y/2);
  float c2 = std::cos(p/2);
  float s2 = std::sin(p/2);
  float c3 = std::cos(r/2);
  float s3 = std::sin(r/2);
  float c1c2 = c1*c2, s1s2 = s1*s2, s1c2 = s1*c2, c1s2 = c1*s2;
  this->w = c1c2 * c3 - s1s2 * s3;
  this->x = s1s2 * c3 + c1c2 * s3;
  this->y = s1c2 * c3 + c1s2 * s3;
  this->z = c1s2 * c3 - s1c2 * s3;
  // Normalise:
  float invlen = 1/length(*this);
  this->x *= invlen;
  this->y *= invlen;
  this->z *= invlen;
  this->w *= invlen;
  return *this;
}

Quaternion& Quaternion::setFromEuler(const Vector3f &e) {
  return setFromEuler(e.pitch, e.yaw, e.roll);
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

} /* namespace glPortal */
