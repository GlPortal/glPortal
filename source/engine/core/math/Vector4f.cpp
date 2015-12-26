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

Vector4f::operator btVector4() const {
  return btVector4(x, y, z, w);
}

Vector4f::Vector4f(const btVector4 &v)
  : x(v.x()), y(v.y()), z(v.z()), w(v.w()) {}

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

Vector4f::Vector4f(const btQuaternion &q)
  : x(q.x()), y(q.y()), z(q.z()), w(q.w()) {}

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
  x = w * x + x * w + y * z - z * y;
  y = w * y + y * w + z * x - x * z;
  z = w * z + z * w + x * y - y * x;
  w = w * w - x * x - y * y - z * z;
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
  const float hY = tetha * 0.5, hP = phi * 0.5, hR = psi * 0.5;
  const float cY = std::cos(hY);
  const float sY = std::sin(hY);
  const float cP = std::cos(hP);
  const float sP = std::sin(hP);
  const float cR = std::cos(hR);
  const float sR = std::sin(hR);
  const float cRcP = cR*cP, cRsP = cR*sP, sRcP = sR*cP, sRsP = sR*sP;
  x = cRsP * cY + sRcP * sY;
  y = cRcP * sY - sRsP * cY;
  z = sRcP * cY - cRsP * sY;
  w = cRcP * cY + sRsP * sY;
  return *this;
}

Quaternion& Quaternion::fromAero(const Vector3f &v) {
  fromAero(v.tetha, v.phi, v.psi);
  return *this;
}

Vector3f Quaternion::toAero() const {
  // http://www.geometrictools.com/Documentation/EulerAngles.pdf
  float r00 = 1.0f - 2.0f*y*y - 2.0f*z*z;
  float r01 = 2.0f*x*y + 2.0f*z*w;
  float r02 = 2.0f*x*z - 2.0f*y*w;
  float r11 = 1.0f - 2.0f*x*x - 2.0f*z*z;
  float r20 = 2.0f*x*z + 2.0f*y*w;
  float r21 = 2.0f*y*z - 2.0f*x*w;
  float r22 = 1.0f - 2.0f*x*x - 2.0f*y*y;
  float thetaX, thetaY, thetaZ;
  if (r21 < +1) {
    if (r21 > -1) {
      thetaX = asin(r21);
      thetaZ = atan2(-r01, r11);
      thetaY = atan2(-r20, r22);
    } else {
      thetaX = -M_PI /2;
      thetaZ = 0;
      thetaY = -atan2(r02, r00);
    }
  } else {
    thetaX = +M_PI/2;
    thetaZ = 0;
    thetaY = atan2(r02, r00);
  }
  return Vector3f(-thetaY, -thetaX, -thetaZ);
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
