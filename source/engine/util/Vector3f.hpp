#ifndef VECTOR3F_H
#define VECTOR3F_H

namespace glPortal {

class Vector3f {
public:
  float x, y, z;

  Vector3f() : Vector3f(0, 0, 0) {}
  Vector3f(float x, float y, float z);
  void set(float x, float y, float z);
  Vector3f *add(Vector3f v);
  Vector3f *subtract(Vector3f v);
  float length();
  Vector3f *normalize();
  void print();
  static Vector3f add(Vector3f v1, Vector3f v2);
  static Vector3f sub(Vector3f v1, Vector3f v2);
  static Vector3f negate(Vector3f v);
};

} /* namespace glPortal */

#endif /* VECTOR3F_HPP */
