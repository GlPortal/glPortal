#ifndef MESHLOADER_HPP
#define MESHLOADER_HPP

#include "Mesh.hpp"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/mesh.h"
#include "assimp/postprocess.h"
#include <map>

namespace glPortal {

class MeshLoader {
public:
  static Mesh getMesh(std::string path);
private:
  static Mesh uploadMesh(const aiMesh* mesh);
  static std::map<std::string, Mesh> meshCache;
};

} /* namespace glPortal */

#endif /* MESHLOADER_HPP */
