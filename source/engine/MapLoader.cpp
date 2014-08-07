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
#include "MeshLoader.hpp"
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
          wall.mesh = MeshLoader::getPortalBox(wall);
          scene->walls.push_back(wall);
          break;
        }
        //Portal walls
        case 'w': {
          Entity wall;
          wall.position.set((values[0] + values[3]) / 2, (values[1] + values[4]) / 2, (values[2] + values[5]) / 2);
          wall.scale.set(abs(values[0] - values[3]), abs(values[1] - values[4]), abs(values[2] - values[5]));
          wall.texture = TextureLoader::getTexture("data/textures/wall.png");
          wall.mesh = MeshLoader::getPortalBox(wall);
          scene->walls.push_back(wall);
          break;
        }
        //Start
        case 's': {
          scene->player.position.set(values[0], values[1], values[2]);
          break;
        }
        //End
        case 'c': {
          Entity door;
          door.position.set(values[0], values[1], values[2]);
          door.rotation.set(values[3], values[4], values[5]);
          door.texture = TextureLoader::getTexture("data/textures/Door.png");
          door.mesh = MeshLoader::getMesh("data/meshes/Door.obj");
          scene->end = door;
          break;
        }
        //Light
        case 'l': {
          Light light;
          light.position.set(values[0], values[1], values[2]);
          light.color.set(values[3], values[4], values[5]);
          scene->lights.push_back(light);
          break;
        }
      }
    }
  }
  file.close();
  return scene;
}

} /* namespace glPortal */
