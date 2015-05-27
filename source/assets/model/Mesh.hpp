#ifndef MESH_HPP
#define MESH_HPP

#include "engine/core/math/Vector3f.hpp"

#include <vector>

namespace glPortal {

class Mesh {
public:
  int handle;
  int numFaces;
  std::vector<Vector3f> vertices;
};

} /* namespace glPortal */

#endif /* MESH_HPP */
