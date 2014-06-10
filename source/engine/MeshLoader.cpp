#include "MeshLoader.hpp"
#include "GL/glew.h"
#include <iostream>

namespace glPortal {

std::map<std::string, Mesh> MeshLoader::meshCache = {};

Mesh MeshLoader::getMesh(std::string path) {
  if(meshCache.find(path) != meshCache.end()) {
    return meshCache.at(path);
  }
  Assimp::Importer importer;
  const aiMesh* mesh = importer.ReadFile(path, aiProcess_Triangulate)->mMeshes[0];
  Mesh m = uploadMesh(mesh);
  meshCache.insert(std::pair<std::string, Mesh>(path, m));
  return m;
}

Mesh MeshLoader::uploadMesh(const aiMesh* mesh) {
  Mesh m;

  //Store face indices in an array
  unsigned int faceArray[mesh->mNumFaces * 3];

  for(unsigned int i = 0; i < mesh->mNumFaces; i++) {
    const aiFace* face = &mesh->mFaces[i];
    faceArray[i * 3 + 0] = face->mIndices[0];
    faceArray[i * 3 + 1] = face->mIndices[1];
    faceArray[i * 3 + 2] = face->mIndices[2];
  }

  //Generate vertex array object
  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  //Store faces in a buffer
  GLuint faceVBO;
  glGenBuffers(1, &faceVBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, faceVBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * mesh->mNumFaces * 3, faceArray, GL_STATIC_DRAW);

  //Store vertices in a buffer
  if(mesh->HasPositions()) {
    GLuint vertexVBO;
    glGenBuffers(1, &vertexVBO);
    glBindBuffer(GL_ARRAY_BUFFER, vertexVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh->mNumVertices * 3, mesh->mVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, 0, 0, 0);
    glEnableVertexAttribArray(0);
  }

  //Store texture coordinates in a buffer
  if(mesh->HasTextureCoords(0)) {
    GLuint textureVBO;
    float* texCoords = (float *) malloc(sizeof(float) * mesh->mNumVertices * 2);
    for (unsigned int k = 0; k < mesh->mNumVertices; ++k) {
      texCoords[k*2]   = mesh->mTextureCoords[0][k].x;
      texCoords[k*2+1] = mesh->mTextureCoords[0][k].y;
    }
    glGenBuffers(1, &textureVBO);
    glBindBuffer(GL_ARRAY_BUFFER, textureVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh->mNumVertices * 2, texCoords, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 2, GL_FLOAT, 0, 0, 0);
    glEnableVertexAttribArray(1);
  }

  //Store normals in a buffer
  if(mesh->HasNormals()) {
    GLuint normalVBO;
    glGenBuffers(1, &normalVBO);
    glBindBuffer(GL_ARRAY_BUFFER, normalVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh->mNumVertices * 3, mesh->mNormals, GL_STATIC_DRAW);
    glVertexAttribPointer(2, 3, GL_FLOAT, 0, 0, 0);
    glEnableVertexAttribArray(2);
  }

  //Unbind the buffers
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  //Store relevant data in the new mesh
  m.handle = vao;
  m.numFaces = mesh->mNumFaces;

  return m;
}

} /* namespace glPortal */
