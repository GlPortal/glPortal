#include "Mesh.hpp"
namespace glPortal {
  namespace engine {
    namespace object {
      Mesh::Mesh(unsigned int numVertices, aiVector3D* vertices, 
		   aiVector3D* normals, unsigned int numFaces, aiFace* faces)
        {
	        this->numVertices = numVertices;
	        this->vertices = vertices;
	        this->normals = normals;
	        this->numIndices = numFaces*3;
	        this->indices = new int[this->numIndices];
	        for(unsigned int i = 0; i < numFaces; i++)
	          {
	            indices[i*3] = faces[i].mIndices[0];
	            indices[i*3+1] = faces[i].mIndices[1];
	            indices[i*3+2] = faces[i].mIndices[2];
	          }
        }

        Mesh::~Mesh(void)
        {
	      delete [] indices;
      }
    }
  }
}
