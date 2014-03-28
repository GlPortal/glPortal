#ifndef __MESH_HPP
#define __MESH_HPP

#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace glPortal {
  namespace engine {
    namespace object {

      class Mesh
      {
      public:
	      Mesh(unsigned int numVertices, aiVector3D* vertices, 
	           aiVector3D* normals, unsigned int numFaces, aiFace* faces);
	      ~Mesh(void);
	      aiVector3D* vertices;
	      aiVector3D* normals;
	      aiVector3D* textureCoords;
	      int* indices;
	      int numVertices;
	      int numIndices;
      };
    }
  }
}
#endif
