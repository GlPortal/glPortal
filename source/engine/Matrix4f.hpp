#ifndef MATRIX4F_H
#define MATRIX4F_H

namespace glPortal {
  namespace engine {
    class Matrix4f {
      public:
        float m00, m01, m02, m03;
        float m10, m11, m12, m13;
        float m20, m21, m22, m23;
        float m30, m31, m32, m33;
        
        Matrix4f();
        Matrix4f(float[]);
        void print();
    };
  }
}
#endif
