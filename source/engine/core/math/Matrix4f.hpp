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
  void translate(const Vector3f &v);
  void translate(float x, float y, float z);
  void rotate(float angle, float x, float y, float z);
  void rotate(const Vector3f &v);
  void scale(const Vector3f &v);
  void scale(float x, float y, float z);
  Vector3f transform(const Vector3f &v);
  void print();
};

float determinant(const Matrix4f &m);
Matrix4f inverse(const Matrix4f &m);

} /* namespace glPortal */

#endif /* MATRIX4F_H */
