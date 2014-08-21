#ifndef MESHLOADER_HPP
#define MESHLOADER_HPP

#include "engine/Mesh.hpp"
#include <map>
#include <string>

struct aiMesh;

namespace glPortal {
class Entity;

class MeshLoader {
public:
  static Mesh getMesh(std::string path);
  static Mesh getPortalBox(Entity wall);
  static Mesh getSubPlane(int x, int y, int width, int height, int w, int h);
private:
  static Mesh uploadMesh(const aiMesh* mesh);
  static std::map<std::string, Mesh> meshCache;
};

} /* namespace glPortal */

#endif /* MESHLOADER_HPP */
