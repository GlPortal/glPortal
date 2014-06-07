#ifndef MATRIX4F_H
#define MATRIX4F_H

#include "Vector3f.hpp"

namespace glPortal {

class Matrix4f {
public:
  float array[16];

  Matrix4f();
  Matrix4f(float[]);
  void setIdentity();
  void translate(Vector3f v);
  void rotate(float angle, float x, float y, float z);
  void scale(Vector3f v);
  void print();
};

} /* namespace glPortal */

#endif /* MATRIX4F_H */
