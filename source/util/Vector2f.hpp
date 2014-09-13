#ifndef VECTOR2F_HPP
#define VECTOR2F_HPP

namespace glPortal {

class Vector2f {
public:
  float x, y;

  Vector2f() :
      Vector2f(0, 0) {
  }
  Vector2f(float x, float y);
  void set(float x, float y);
  Vector2f *add(Vector2f v);
  Vector2f *subtract(Vector2f v);
  float length();
  Vector2f *normalize();
  void print();
  static Vector2f add(Vector2f v1, Vector2f v2);
  static Vector2f sub(Vector2f v1, Vector2f v2);
  static Vector2f negate(Vector2f v);
};

} /* namespace glPortal */

#endif /* VECTOR2F_HPP */
