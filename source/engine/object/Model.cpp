#include "Model.hpp"
#include <GL/glew.h>

namespace glPortal {
  namespace engine {
    namespace object {
      Model::Model(const string & filename) {
	      importer = new Assimp::Importer();
	      scene = importer->ReadFile(filename,
				         aiProcess_Triangulate            |
				         aiProcess_GenSmoothNormals	      |
				         aiProcess_JoinIdenticalVertices  |
				         aiProcess_ImproveCacheLocality   |
				         aiProcess_SortByPType);

	      for(unsigned int i = 0; i < scene->mNumMeshes; i++)
	        {
	          Mesh* tempMesh = new Mesh(scene->mMeshes[i]->mNumVertices,
				                              scene->mMeshes[i]->mVertices,
				                              scene->mMeshes[i]->mNormals,
				                              scene->mMeshes[i]->mNumFaces,
				                              scene->mMeshes[i]->mFaces);
		
	          meshes.push_back(tempMesh);
	        }

	      this->loadTextures(scene, scene->mRootNode);
      }

      void Model::draw() {
	for(unsigned int i = 0; i < meshes.size(); i++) {
          Mesh *mesh = meshes[i];	    
          glEnableClientState(GL_NORMAL_ARRAY);
          glEnableClientState(GL_VERTEX_ARRAY);
          glNormalPointer(GL_FLOAT, 0, mesh->normals);
          glColor3f(0.0f,0.0f,1.0f);   
          glVertexPointer(3, GL_FLOAT, 0, mesh->vertices);
          glDrawElements(GL_TRIANGLES, mesh->numIndices, GL_UNSIGNED_INT, mesh->indices);
          glDisableClientState(GL_VERTEX_ARRAY);
          glDisableClientState(GL_NORMAL_ARRAY);
          glColor3f(1,1,1);
	}
      }

      void Model::setPosition(int x, int y, int z){
	      //do nothing
      }


      void Model::loadTextures(const struct aiScene *sc, const struct aiNode* nd){
      }
    }
  }
}
