#include "MapLoader.hpp"
#include <fstream>
#include <sstream>
#include <cmath>
#include <cstdlib>
#include <iostream>

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
      float values[6];

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
          scene->lights.push_back(light);
        }
      }
    }
  }
  file.close();
  return scene;
}

} /* namespace glPortal */
