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

  // Generate vertex array object
  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);


  // Prepare vertex indices VBO
  GLuint idxVBO;
  glGenBuffers(1, &idxVBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idxVBO);
  
  // Store face indices in an array
  unsigned int *faceArray = new unsigned int[mesh->mNumFaces * 3];
  for (unsigned int f = 0; f < mesh->mNumFaces; ++f) {
    const aiFace &face = mesh->mFaces[f];
    std::memcpy(&faceArray[f*3], face.mIndices, 3*sizeof(face.mIndices[0]));
  }

  // Upload vertex indices to the GPU
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,
      sizeof(unsigned int) * mesh->mNumFaces * 3, faceArray, GL_STATIC_DRAW);

  // Once data is uploaded to GPU, no need to keep it in memory
  delete [] faceArray;


  // Prepare vertex data VBO
  GLuint vbo;
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);

  // Cache vertex attributes existence to avoid dereferencing and calling every time
  bool hasPositions = mesh->HasPositions(),
       hasTexCoords = mesh->HasTextureCoords(0),
       hasNormals   = mesh->HasNormals(),
       hasTangents  = mesh->HasTangentsAndBitangents();

  // Use them to determine the vertex definition size
  unsigned int vtxSize =
    + (hasPositions ? sizeof(float)*3 : 0)
    + (hasTexCoords ? sizeof(float)*2 : 0)
    + (hasNormals   ? sizeof(float)*3 : 0)
    + (hasTangents  ? sizeof(float)*3 : 0);

  // Create a buffer to store vertex data, with determined size since we now know it
  TightDataPacker data(mesh->mNumVertices * vtxSize);

  if (hasPositions) {
    // Since we keep the vertices in the model, better determine the storage size once and for all
    m.vertices.resize(mesh->mNumVertices);
  }

  // Process each vertex and store its data
  for (unsigned int i = 0; i < mesh->mNumVertices; ++i) {
    if (hasPositions) {
      const aiVector3D &v = mesh->mVertices[i];
      m.vertices[i] = Vector3f(v.x, v.y, v.z);
      data << v.x << v.y << v.z;
    }

    if (hasTexCoords) {
      const aiVector3D &t = mesh->mTextureCoords[0][i];
      data << t.x
           << 1.f - t.y; // Y must be flipped due to OpenGL's coordinate system
    }

    if (hasNormals) {
      const aiVector3D &n = mesh->mNormals[i];
      data << n.x << n.y << n.z;
    }

    if (hasTangents) {
      const aiVector3D &t = mesh->mTangents[i];
      data << t.x << t.y << t.z;
    }
  }

  // Upload vertex data to the GPU
  glBufferData(GL_ARRAY_BUFFER, data.getSize(), data.getDataPtr(), GL_STATIC_DRAW);

  // Describe the vertex format we have
  intptr_t offset = 0;
  if (hasPositions) {
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vtxSize, (GLvoid*)offset);
    glEnableVertexAttribArray(0);
    offset += sizeof(float)*3;
  }
  if (hasTexCoords) {
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, vtxSize, (GLvoid*)offset);
    glEnableVertexAttribArray(1);
    offset += sizeof(float)*2;
  }
  if (hasNormals) {
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, vtxSize, (GLvoid*)offset);
    glEnableVertexAttribArray(2);
    offset += sizeof(float)*3;
  }
  if (hasTangents) {
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, vtxSize, (GLvoid*)offset);
    glEnableVertexAttribArray(3);
    // offset += sizeof(float)*3;
  }

  // Unbind the buffers
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  // Store relevant data in the new mesh
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
    vboSize = vtxSize * 3 /*verts*/ * 2 /*tris*/ * 6; /*faces*/
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
    3, 1, 5, 3, 5, 7, // Front
    7, 5, 4, 7, 4, 6, // Left
    6, 4, 0, 6, 0, 2, // Back
    2, 0, 1, 2, 1, 3, // Right
    2, 3, 7, 2, 7, 6, // Top
    1, 0, 4, 1, 4, 5  // Bottom
  };

  const Transform &t = wall.getComponent<Transform>();
  const Vector3f &scale = t.getScale();
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
    0, 3, 5, 0, 5, 1,
    0, 3, 7, 0, 7, 2,
    0, 3, 5, 0, 5, 1,
    0, 3, 7, 0, 7, 2,
    0, 4, 6, 0, 6, 1,
    0, 4, 6, 0, 6, 1
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
    0, 0, 0, 0, 0, 0,
    1, 1, 1, 1, 1, 1,
    2, 2, 2, 2, 2, 2,
    3, 3, 3, 3, 3, 3,
    4, 4, 4, 4, 4, 4,
    5, 5, 5, 5, 5, 5
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
    0, 0, 0, 0, 0, 0,
    1, 1, 1, 1, 1, 1,
    2, 2, 2, 2, 2, 2,
    3, 3, 3, 3, 3, 3,
    4, 4, 4, 4, 4, 4,
    5, 5, 5, 5, 5, 5
  };
  constexpr unsigned int
    texCoordsOffset = coordsSize,
    normalsOffset = texCoordsOffset + texcSize,
    tangentsOffset = normalsOffset + normalsSize;
  for (int i = 0; i < 36; ++i) {
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

  static const int8_t vertices[4][3] = {
    {0, -1, 0}, {1, -1, 0},
    {1,  0, 0}, {0,  0, 0}
  };
  const float texCoords[4][2] = {
    {(float)x/w, (float)y/h},
    {(float)(x+width)/w, (float)y/h},
    {(float)(x+width)/w, (float)(y+height)/h},
    {(float)x/w, (float)(y+height)/h}
  };

  static const uint8_t
    vi[6] = { 0, 1, 3, 2, 3, 1 },
    ti[6] = { 3, 2, 0, 1, 0, 2 };

  constexpr unsigned int vtxSize = 3*sizeof(int8_t) + 2*sizeof(float);
  TightDataPacker data(6*vtxSize);
  for (int i = 0; i < 6; ++i) {
    const int8_t *v = vertices[vi[i]];
    data << v[0] << v[1] << v[2];
    const float *t = texCoords[ti[i]];
    data << t[0] << t[1];
  }

  assert(data.getSize() == 6*vtxSize);

  // Put the vertex buffer into the VAO
  GLuint vbo;
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, data.getSize(), data.getDataPtr(), GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_BYTE, GL_FALSE, vtxSize, nullptr);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, vtxSize, (GLvoid*)(3*sizeof(int8_t)));
  glEnableVertexAttribArray(1);

  // Unbind the buffers
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  // Store relevant data in the new mesh
  mesh.handle = vao;
  mesh.numFaces = 2;

  return mesh;
}

} /* namespace glPortal */
