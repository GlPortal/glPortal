#ifndef LIGHT_HPP
#define LIGHT_HPP

#include "util/Vector3f.hpp"

using namespace util;

class Light {
  public:
    Light() {}
    Light(float x, float y, float z);
    Vector3f position;
};

#endif

