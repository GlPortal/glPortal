#ifndef MATRIX4F_H
#define MATRIX4F_H

namespace glPortal {
class Vector3f;

class Matrix4f {
public:
  float array[16];

  Matrix4f();
  Matrix4f(float[]);
  void setIdentity();
  void translate(Vector3f v);
  void rotate(float angle, float x, float y, float z);
  void rotate(Vector3f v);
  void scale(Vector3f v);
  void print();
};

} /* namespace glPortal */

#endif /* MATRIX4F_H */
