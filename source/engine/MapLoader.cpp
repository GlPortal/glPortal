#include "MapLoader.hpp"

#include <GL/glew.h>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <istream>
#include <sstream>
#include <vector>

#include "../Player.hpp"
#include "../Scene.hpp"
#include "Light.hpp"
#include "Mesh.hpp"
#include "Texture.hpp"
#include "TextureLoader.hpp"
#include "util/Vector2f.hpp"
#include "util/Vector3f.hpp"

using namespace std;

namespace glPortal {

Scene* MapLoader::getScene(std::string path) {
  Scene* scene = new Scene();

  ifstream file(path);
  string line;

  while(getline(file, line)) {
    stringstream stream(line);
    char type;

    if(line.length() > 0) {
      stream >> type;
      type = line[0];
      float values[8];

      for(int i = 0; i < 6; i++) {
        string string;
        stream >> string;
        values[i] = static_cast<float>(atof(string.c_str()));
      }

      switch(type) {
        //Walls
        case 't': {
          Entity wall;
          wall.position.set((values[0] + values[3]) / 2, (values[1] + values[4]) / 2, (values[2] + values[5]) / 2);
          wall.scale.set(abs(values[0] - values[3]), abs(values[1] - values[4]), abs(values[2] - values[5]));
          wall.texture = TextureLoader::getTexture("data/textures/tiles.png");
          wall.mesh = getBox(wall);
          scene->walls.push_back(wall);
          break;
        }
        //Portal walls
        case 'w': {
          Entity wall;
          wall.position.set((values[0] + values[3]) / 2, (values[1] + values[4]) / 2, (values[2] + values[5]) / 2);
          wall.scale.set(abs(values[0] - values[3]), abs(values[1] - values[4]), abs(values[2] - values[5]));
          wall.texture = TextureLoader::getTexture("data/textures/wall.png");
          wall.mesh = getBox(wall);
          scene->walls.push_back(wall);
          break;
        }
        //Start
        case 's': {
          scene->player.position.set(values[0], values[1], values[2]);
          break;
        }
        //Light
        case 'l': {
          Light light;
          light.position.set(values[0], values[1], values[2]);
          light.color.set(values[3], values[4], values[5]);
          scene->lights.push_back(light);
        }
      }
    }
  }
  file.close();
  return scene;
}

Mesh MapLoader::getBox(Entity wall) {
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

} /* namespace glPortal */
