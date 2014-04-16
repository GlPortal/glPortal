#ifndef VECTOR3F_H
#define VECTOR3F_H

namespace util {
  class Vector3f {
    public:
      float x, y, z;
      
      Vector3f() : Vector3f(0, 0, 0) {}
      Vector3f(float x, float y, float z);
      Vector3f *set(float x, float y, float z);
      Vector3f *add(Vector3f v);
      Vector3f *subtract(Vector3f v);
      float length();
      Vector3f *normalize();
  };
}

#endif
