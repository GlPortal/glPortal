#include "Vector4f.hpp"

#include <bullet/LinearMath/btQuaternion.h>
#include <bullet/LinearMath/btVector3.h>
#include <engine/core/math/Vector2f.hpp>
#include <engine/core/math/Vector3f.hpp>

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

Vector4f::operator btQuaternion() const {
  return btQuaternion(x, y, z, w);
}

} /* namespace glPortal */
