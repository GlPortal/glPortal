#ifndef ENTITY_HPP
#define ENTITY_HPP

#include "Mesh.hpp"
#include "util/Vector3f.hpp"

namespace glPortal {

class Entity {
public:
  Entity() : position(0, 0, 0), rotation(0, 0, 0), scale(1, 1, 1) {}
  Mesh mesh;

  Vector3f position;
  Vector3f rotation;
  Vector3f scale;
};

} /* namespace glPortal */

#endif /* ENTITY_HPP */
