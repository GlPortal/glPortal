#include "MeshLoader.hpp"

#include <stdlib.h>
#include <utility>
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/mesh.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/vector3.h>
#include <epoxy/gl.h>

#include <engine/env/Environment.hpp>
#include <engine/Entity.hpp>
#include <engine/core/math/Vector2f.hpp>
#include <engine/core/math/Vector3f.hpp>
#include <engine/core/gl/TightDataPacker.hpp>
#include <engine/component/Transform.hpp>

namespace glPortal {

std::map<std::string, Mesh> MeshLoader::meshCache = { };

Mesh& MeshLoader::getMesh(const std::string &path) {
  auto it = meshCache.find(path);
  if (it != meshCache.end()) {
    return it->second;
  }
  std::string fpath = Environment::getDataDir() + "/meshes/" + path;
  Assimp::Importer importer;
  int flags = aiProcess_Triangulate | aiProcess_GenNormals | aiProcess_CalcTangentSpace;
  const aiMesh *mesh = importer.ReadFile(fpath, flags)->mMeshes[0];
  Mesh m = uploadMesh(mesh);
  auto inserted = meshCache.insert(std::pair<std::string, Mesh>(path, m));
  // Return reference to newly inserted Mesh
  return inserted.first->second;
}

Mesh MeshLoader::uploadMesh(const aiMesh *mesh) {
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
    m.vertices.resize(mesh->mNumVertices);
    for (unsigned int j = 0; j < mesh->mNumVertices; j++) {
      Vector3f v(mesh->mVertices[j].x, mesh->mVertices[j].y, mesh->mVertices[j].z);
      m.vertices[j] = v;
    }

    GLuint vertexVBO;
    glGenBuffers(1, &vertexVBO);
    glBindBuffer(GL_ARRAY_BUFFER, vertexVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh->mNumVertices * 3, &m.vertices[0], GL_STATIC_DRAW);
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

  //Store tangents in a buffer
  if (mesh->HasTangentsAndBitangents()) {
    GLuint tangentVBO;
    glGenBuffers(1, &tangentVBO);
    glBindBuffer(GL_ARRAY_BUFFER, tangentVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh->mNumVertices * 3, mesh->mTangents, GL_STATIC_DRAW);
    glVertexAttribPointer(3, 3, GL_FLOAT, 0, 0, 0);
    glEnableVertexAttribArray(3);
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

Mesh MeshLoader::getPortalBox(const Entity &wall) {
  Mesh mesh;

  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  /* == Static part: vertices, normals and tangents == */

  constexpr unsigned int
    coordsSize = sizeof(float)*3,
    texcSize = sizeof(float)*2,
    normalsSize = sizeof(int8_t)*3,
    tangentsSize = sizeof(int8_t)*3,
    vtxSize = coordsSize + texcSize + normalsSize + tangentsSize,
    vboSize = vtxSize * 3 /*verts*/ * 2 /*tris*/ * 6 /*faces*/;
  TightDataPacker data(vboSize);

  static const float vertices[8][3] = {
    {-0.5f, -0.5f, -0.5f},
    {-0.5f, -0.5f,  0.5f},
    {-0.5f,  0.5f, -0.5f},
    {-0.5f,  0.5f,  0.5f},
    { 0.5f, -0.5f, -0.5f},
    { 0.5f, -0.5f,  0.5f},
    { 0.5f,  0.5f, -0.5f},
    { 0.5f,  0.5f,  0.5f}
  };
  static const uint8_t vi[36] = {
    3,1,5,3,5,7, // Front
    7,5,4,7,4,6, // Left
    6,4,0,6,0,2, // Back
    2,0,1,2,1,3, // Right
    2,3,7,2,7,6, // Top
    1,0,4,1,4,5  // Bottom
  };

  const Transform &t = wall.getComponent<Transform>();
  const Vector3f &position = t.position;
  const Vector3f &scale = t.scale;
  const float texCoords[8][2] = {
    {0, 0},
    {scale.x, 0},
    {scale.z, 0},
    {0, scale.y},
    {0, scale.z},
    {scale.x, scale.y},
    {scale.x, scale.z},
    {scale.z, scale.y}
  };
  static const uint8_t ti[36] = {
    0,3,5,0,5,1,
    0,3,7,0,7,2,
    0,3,5,0,5,1,
    0,3,7,0,7,2,
    0,4,6,0,6,1,
    0,4,6,0,6,1
  };

  static const int8_t normals[6][3] = {
    { 0,  0,  1},
    { 1,  0,  0},
    { 0,  0, -1},
    {-1,  0,  0},
    { 0,  1,  0},
    { 0, -1,  0}
  };
  static const uint8_t ni[36] = {
    0,0,0,0,0,0,
    1,1,1,1,1,1,
    2,2,2,2,2,2,
    3,3,3,3,3,3,
    4,4,4,4,4,4,
    5,5,5,5,5,5
  };

  static const int8_t tangents[6][3] = {
    { 0,  1,  0},
    { 0,  0,  1},
    { 0, -1,  0},
    { 0,  0, -1},
    { 1,  0,  0},
    {-1,  0,  0}
  };
  static const uint8_t tai[36] = {
    0,0,0,0,0,0,
    1,1,1,1,1,1,
    2,2,2,2,2,2,
    3,3,3,3,3,3,
    4,4,4,4,4,4,
    5,5,5,5,5,5
  };
  constexpr unsigned int
    texCoordsOffset = coordsSize,
    normalsOffset = texCoordsOffset + texcSize,
    tangentsOffset = normalsOffset + normalsSize;
  for(int i = 0; i < 36; ++i) {
    const float *v = vertices[vi[i]];
    data << v[0] << v[1] << v[2];
    
    const float *t = texCoords[ti[i]];
    data << t[0] << t[1];

    const int8_t *n = normals[ni[i]];
    data << n[0] << n[1] << n[2];

    const int8_t *ta = tangents[tai[i]];
    data << ta[0] << ta[1] << ta[2];
  }
  mesh.vertices.resize(8);
  for (int i = 0; i < 8; ++i) {
    const float *v = vertices[i];
    mesh.vertices[i] = Vector3f(v[0], v[1], v[2]);
  }

  assert(data.getSize() == vboSize);

  GLuint vbo;
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, vboSize, data.getDataPtr(), GL_STATIC_DRAW);
  // Vertices
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vtxSize, nullptr);
  glEnableVertexAttribArray(0);
  // Tex coords
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, vtxSize, (GLvoid*)texCoordsOffset);
  glEnableVertexAttribArray(1);
  // Normals
  glVertexAttribPointer(2, 3, GL_BYTE, GL_FALSE, vtxSize, (GLvoid*)normalsOffset);
  glEnableVertexAttribArray(2);
  // Tangents
  glVertexAttribPointer(3, 3, GL_BYTE, GL_FALSE, vtxSize, (GLvoid*)tangentsOffset);
  glEnableVertexAttribArray(3);

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
