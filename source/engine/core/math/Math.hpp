#ifndef MATH_HPP
#define MATH_HPP

namespace glPortal {

class Vector3f;

class Math {
public:
  static const float PI_RND;
  static float toDegrees(float radius);
  static float toRadians(float degrees);
  static Vector3f toDirection(Vector3f v);
  static Vector3f toEuler(Vector3f direction);
};

} /* namespace glPortal */

#endif /* MATH_HPP */
