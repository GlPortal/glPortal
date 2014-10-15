#ifndef VECTOR3F_HPP
#define VECTOR3F_HPP

namespace glPortal {

class Vector3f {
public:
  float x, y, z;

  Vector3f() :
      Vector3f(0, 0, 0) {
  }
  Vector3f(float x, float y, float z);
  void set(float x, float y, float z);
  void set(Vector3f v);
  Vector3f *add(Vector3f v);
  Vector3f *sub(Vector3f v);
  Vector3f *scale(float scale);
  float length();
  Vector3f *normalize();
  void print();
  static Vector3f add(Vector3f v1, Vector3f v2);
  static Vector3f sub(Vector3f v1, Vector3f v2);
  static Vector3f scale(Vector3f v, float scale);
  static Vector3f negate(Vector3f v);
};

} /* namespace glPortal */

#endif /* VECTOR3F_HPP */
