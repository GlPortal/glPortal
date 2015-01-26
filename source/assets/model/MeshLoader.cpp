#include "MeshLoader.hpp"

#include <assimp/Importer.hpp>
#include <assimp/mesh.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/vector3.h>
#include <GL/glew.h>
#include <stdlib.h>
#include <utility>
#include <vector>

#include "engine/env/Environment.hpp"
#include "engine/Entity.hpp"
#include "engine/core/math/Vector2f.hpp"
#include "engine/core/math/Vector3f.hpp"
#include <cstdio>

namespace glPortal {

std::map<std::string, Mesh> MeshLoader::meshCache = { };

Mesh MeshLoader::getMesh(std::string path) {
  path = Environment::getDataDir() + "/meshes/" + path;
  if (meshCache.find(path) != meshCache.end()) {
    return meshCache.at(path);
  }
  Assimp::Importer importer;
  int flags = aiProcess_Triangulate | aiProcess_GenNormals;
  const aiMesh* mesh = importer.ReadFile(path, flags)->mMeshes[0];
  Mesh m = uploadMesh(mesh);
  meshCache.insert(std::pair<std::string, Mesh>(path, m));
  return m;
}

Mesh MeshLoader::uploadMesh(const aiMesh* mesh) {
  Mesh m;

  //Store face indices in an array
  std::vector<unsigned int> faceArray;
  
  for (unsigned int j = 0; j < mesh->mNumFaces; j++) {
    const aiFace face = mesh->mFaces[j];
    faceArray.push_back(face.mIndices[0]);
    faceArray.push_back(face.mIndices[1]);
    faceArray.push_back(face.mIndices[2]);
  }

  //Generate vertex array object
  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  //Store faces in a buffer
  GLuint faceVBO;
  glGenBuffers(1, &faceVBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, faceVBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,
      sizeof(unsigned int) * mesh->mNumFaces * 3, &faceArray[0], GL_STATIC_DRAW);

  //Store vertices in a buffer
  if (mesh->HasPositions()) {
    GLuint vertexVBO;
    glGenBuffers(1, &vertexVBO);
    glBindBuffer(GL_ARRAY_BUFFER, vertexVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh->mNumVertices * 3, mesh->mVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, 0, 0, 0);
    glEnableVertexAttribArray(0);
  }

  //Store texture coordinates in a buffer
  if (mesh->HasTextureCoords(0)) {
    GLuint textureVBO;
    float* texCoords = (float *) malloc(sizeof(float) * mesh->mNumVertices * 2);
    for (unsigned int k = 0; k < mesh->mNumVertices; ++k) {
      texCoords[k * 2] = mesh->mTextureCoords[0][k].x;
      texCoords[k * 2 + 1] = 1 - mesh->mTextureCoords[0][k].y; //Y must be flipped due to OpenGL's coordinate system
    }
    glGenBuffers(1, &textureVBO);
    glBindBuffer(GL_ARRAY_BUFFER, textureVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh->mNumVertices * 2, texCoords, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 2, GL_FLOAT, 0, 0, 0);
    glEnableVertexAttribArray(1);
  }

  //Store normals in a buffer
  if (mesh->HasNormals()) {
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

Mesh MeshLoader::getPortalBox(Entity wall) {
  Mesh mesh;

  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  //Vertices
  Vector3f vertices[8] = {Vector3f(-0.5, -0.5f, -0.5f),
                          Vector3f(-0.5f, -0.5f, 0.5f),
                          Vector3f(-0.5f, 0.5f, -0.5f),
                          Vector3f(-0.5f, 0.5f, 0.5f),
                          Vector3f(0.5f, -0.5f, -0.5f),
                          Vector3f(0.5f, -0.5f, 0.5f),
                          Vector3f(0.5f, 0.5f, -0.5f),
                          Vector3f(0.5f, 0.5f, 0.5f)};

  float vi[36] = {3,1,5,3,5,7, 7,5,4,7,4,6, 6,4,0,6,0,2, 2,0,1,2,1,3, 2,3,7,2,7,6, 1,0,4,1,4,5};
  float vertexBuffer[36 * 3];
  for(int i = 0; i < 36; i++) {
    int index = vi[i];
    vertexBuffer[i * 3 + 0] = vertices[index].x;
    vertexBuffer[i * 3 + 1] = vertices[index].y;
    vertexBuffer[i * 3 + 2] = vertices[index].z;
  }
  //Put the vertex buffer into the VAO
  GLuint vertexVBO;
  glGenBuffers(1, &vertexVBO);
  glBindBuffer(GL_ARRAY_BUFFER, vertexVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 36 * 3, vertexBuffer, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, 0, 0, 0);
  glEnableVertexAttribArray(0);

  //Texture coordinates
  Vector2f texCoords[8] = {Vector2f(0, 0),
                          Vector2f(wall.scale.x, 0),
                          Vector2f(wall.scale.z, 0),
                          Vector2f(0, wall.scale.y),
                          Vector2f(0, wall.scale.z),
                          Vector2f(wall.scale.x, wall.scale.y),
                          Vector2f(wall.scale.x, wall.scale.z),
                          Vector2f(wall.scale.z, wall.scale.y)};

  float ti[36] = {0,3,5,0,5,1, 0,3,7,0,7,2, 0,3,5,0,5,1, 0,3,7,0,7,2, 0,4,6,0,6,1, 0,4,6,0,6,1};
  float textureBuffer[36 * 2];
  for(int i = 0; i < 36; i++) {
    int index = ti[i];
    textureBuffer[i * 2 + 0] = texCoords[index].x;
    textureBuffer[i * 2 + 1] = texCoords[index].y;
  }
  //Put the texture buffer into the VAO
  GLuint textureVBO;
  glGenBuffers(1, &textureVBO);
  glBindBuffer(GL_ARRAY_BUFFER, textureVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 36 * 2, textureBuffer, GL_STATIC_DRAW);
  glVertexAttribPointer(1, 2, GL_FLOAT, 0, 0, 0);
  glEnableVertexAttribArray(1);

  //Normals
  Vector3f normals[6] = {Vector3f(0, 0, 1),
                         Vector3f(1, 0, 0),
                         Vector3f(0, 0, -1),
                         Vector3f(-1, 0, 0),
                         Vector3f(0, 1, 0),
                         Vector3f(0, -1, 0)};

  float ni[36] = {0,0,0,0,0,0, 1,1,1,1,1,1, 2,2,2,2,2,2, 3,3,3,3,3,3, 4,4,4,4,4,4, 5,5,5,5,5,5};
  float normalBuffer[36 * 3];
  for(int i = 0; i < 36; i++) {
    int index = ni[i];
    normalBuffer[i * 3 + 0] = normals[index].x;
    normalBuffer[i * 3 + 1] = normals[index].y;
    normalBuffer[i * 3 + 2] = normals[index].z;
  }
  //Put the normal buffer into the VAO
  GLuint normalVBO;
  glGenBuffers(1, &normalVBO);
  glBindBuffer(GL_ARRAY_BUFFER, normalVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 36 * 3, normalBuffer, GL_STATIC_DRAW);
  glVertexAttribPointer(2, 3, GL_FLOAT, 0, 0, 0);
  glEnableVertexAttribArray(2);

  //Unbind the buffers
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  //Store relevant data in the new mesh
  mesh.handle = vao;
  mesh.numFaces = 12;

  return mesh;
}

Mesh MeshLoader::getSubPlane(int x, int y, int width, int height, int w, int h) {
  Mesh mesh;

  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  Vector3f vertices[4] = {Vector3f(0, -1, 0), Vector3f(1, -1, 0),
                          Vector3f(1, 0, 0), Vector3f(0, 0, 0)};
  Vector2f texCoords[4] = {Vector2f((float)x/w, (float)y/h),
                           Vector2f((float)(x+width)/w, (float)y/h),
                           Vector2f((float)(x+width)/w, (float)(y+height)/h),
                           Vector2f((float)x/w, (float)(y+height)/h)};

  float vi[6] = {0,1,3,2,3,1};
  float vertexBuffer[6 * 3];
  for(int i = 0; i < 6; i++) {
    int index = vi[i];
    vertexBuffer[i * 3 + 0] = vertices[index].x;
    vertexBuffer[i * 3 + 1] = vertices[index].y;
    vertexBuffer[i * 3 + 2] = vertices[index].z;
  }

  float ti[6] = {3,2,0,1,0,2};
  float textureBuffer[6 * 2];
  for(int i = 0; i < 6; i++) {
    int index = ti[i];
    textureBuffer[i * 2 + 0] = texCoords[index].x;
    textureBuffer[i * 2 + 1] = texCoords[index].y;
  }

  //Put the vertex buffer into the VAO
  GLuint vertexVBO;
  glGenBuffers(1, &vertexVBO);
  glBindBuffer(GL_ARRAY_BUFFER, vertexVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 3, vertexBuffer, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, 0, 0, 0);
  glEnableVertexAttribArray(0);

  //Put the texture buffer into the VAO
  GLuint textureVBO;
  glGenBuffers(1, &textureVBO);
  glBindBuffer(GL_ARRAY_BUFFER, textureVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 2, textureBuffer, GL_STATIC_DRAW);
  glVertexAttribPointer(1, 2, GL_FLOAT, 0, 0, 0);
  glEnableVertexAttribArray(1);

  //Unbind the buffers
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  //Store relevant data in the new mesh
  mesh.handle = vao;
  mesh.numFaces = 2;

  return mesh;
}

} /* namespace glPortal */
