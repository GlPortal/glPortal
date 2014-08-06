#ifndef MESHLOADER_HPP
#define MESHLOADER_HPP

#include <map>
#include <string>

#include "Mesh.hpp"

struct aiMesh;

namespace glPortal {
class Entity;

class MeshLoader {
public:
  static Mesh getMesh(std::string path);
  static Mesh getPortalWall(Entity wall);
private:
  static Mesh uploadMesh(const aiMesh* mesh);
  static std::map<std::string, Mesh> meshCache;
};

} /* namespace glPortal */

#endif /* MESHLOADER_HPP */
