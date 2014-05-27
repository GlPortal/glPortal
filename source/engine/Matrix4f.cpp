#include "Matrix4f.hpp"
#include <stdio.h>

namespace glPortal {
  namespace engine {
    Matrix4f::Matrix4f() {
      m00 = 1; m01 = 0; m02 = 0; m03 = 0;
      m10 = 0; m11 = 1; m12 = 0; m13 = 0;
      m20 = 0; m21 = 0; m22 = 1; m23 = 0;
      m30 = 0; m31 = 0; m32 = 0; m33 = 1;
    }

    Matrix4f::Matrix4f(float array[]) {
      m00 = array[0]; m01 = array[1]; m02 = array[2]; m03 = array[3];
      m10 = array[4]; m11 = array[5]; m12 = array[6]; m13 = array[7];
      m20 = array[8]; m21 = array[9]; m22 = array[10]; m23 = array[11];
      m30 = array[12]; m31 = array[13]; m32 = array[14]; m33 = array[15];
    }

    void Matrix4f::print() {
      printf("%f %f %f %f\n", m00, m01, m02, m03);
      printf("%f %f %f %f\n", m10, m11, m12, m13);
      printf("%f %f %f %f\n", m20, m21, m22, m23);
      printf("%f %f %f %f\n", m30, m31, m32, m33);
    }
  }
}
