#include "FontLoader.hpp"

#include <fstream>
#include <sstream>
#include <cstdlib>
#include <cstdio>

#include <assets/model/MeshLoader.hpp>
#include <assets/texture/TextureLoader.hpp>

#include <engine/env/Environment.hpp>

namespace glPortal {

std::map<std::string, Font> FontLoader::fontCache = {};

Font FontLoader::getFont(std::string name) {
  std::string path = Environment::getDataDir() + "/fonts/" + name + ".txt";
  if(fontCache.find(path) != fontCache.end()) {
    return fontCache.at(path);
  }
  Font font = loadFont(path, name);
  fontCache.insert(std::pair<std::string, Font>(path, font));
  return font;
}

Font FontLoader::loadFont(std::string path, std::string name) {
  Font font;
  std::ifstream input(path);
  for(std::string line; getline(input, line);) {
    stringstream stream(line);

    if(line.length() > 0) {
      std::string tokens[9];

      for(int i = 0; i < 9; i++) {
        stream >> tokens[i];
      }

      if(tokens[0] == "char") {
        //Delete all the letters from the tokens
        for(int i = 1; i < 9; i++) {
          //Find the start of the digits
          int pos = tokens[i].find('=', 0) + 1;
          //Erase the rest
          tokens[i].erase(0, pos);
        }

        Letter letter;
        int id = atoi(tokens[1].c_str());
        letter.x = atoi(tokens[2].c_str());
        letter.y = atoi(tokens[3].c_str());
        letter.width = atoi(tokens[4].c_str());
        letter.height = atoi(tokens[5].c_str());
        letter.xOffset = atof(tokens[6].c_str());
        letter.yOffset = atof(tokens[7].c_str());
        letter.advance = atof(tokens[8].c_str());

        //Load the mesh
        Texture texture = TextureLoader::getTexture(name + ".png");
        letter.mesh = MeshLoader::getSubPlane(letter.x, letter.y, letter.width, letter.height, texture.width, texture.height);

        font.letters.insert(std::pair<int, Letter>(id, letter));
      }
    }
  }

  return font;
}

} /* namespace glPortal */
