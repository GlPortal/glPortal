#ifndef __MODEL_HPP
#define __MODEL_HPP

#include "../../Exception.hpp"

#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/glext.h>
//#include <GL/glfw.h>
#include <stdio.h>
#include <string>
#include "Mesh.hpp"
#include <vector>

#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>


using namespace std;

namespace glPortal {
  namespace engine {
    namespace object {

      class Model{
      public:
	Model(const string & filename);
	void draw();
	void changePosition(int x, int y, int z);
      private:
	vector<Mesh*> meshes;
	const aiScene* scene;
	Assimp::Importer* importer;
      };
    }
  }
}
#endif
