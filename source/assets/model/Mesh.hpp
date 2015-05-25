#ifndef MESH_HPP
#define MESH_HPP

namespace glPortal {

class Mesh {
public:
  Mesh();
  int handle;
  int numFaces;
private:
  friend class MeshLoader;
  Mesh(int handle);
};

} /* namespace glPortal */

#endif /* MESH_HPP */
