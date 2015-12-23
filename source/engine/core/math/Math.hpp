#ifndef MATH_HPP
#define MATH_HPP

namespace glPortal {

class Vector3f;
class Quaternion;

class Math {
public:
  static constexpr float PI_RND = 3.1415926535897932384626433832795028841971693993751058209749445923078164062862089;
  static constexpr float DEG_TO_RAD = PI_RND / 180;
  static constexpr float RAD_TO_DEG = 180 / PI_RND;

  static Vector3f toDirection(const Quaternion &orientation);
  static Vector3f toEuler(const Vector3f &direction);

  template <typename T>
  static T clamp(T v, T low, T high) {
    if (v < low) {
      return low;
    }
    else if (v > high) {
      return high;
    }
    return v;
  }
};

constexpr inline float deg(float rad) {
  return rad * Math::RAD_TO_DEG;
}

constexpr inline float rad(float deg) {
  return deg * Math::DEG_TO_RAD;
}

constexpr inline float sign(float v) {
  if (v > 0.f) return 1.f;
  if (v < 0.f) return -1.f;
  return 0.f;
}

} /* namespace glPortal */

#endif /* MATH_HPP */
