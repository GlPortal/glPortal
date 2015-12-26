#include "FontLoader.hpp"

#include <fstream>
#include <sstream>
#include <cstdlib>
#include <cstdio>
#include <string>

#include <assets/model/MeshLoader.hpp>
#include <assets/texture/TextureLoader.hpp>

#include <engine/env/Environment.hpp>

namespace glPortal {

std::map<std::string, Font> FontLoader::fontCache = {};

Font& FontLoader::getFont(const std::string &name) {
  auto it = fontCache.find(name);
  if (it != fontCache.end()) {
    return it->second;
  }
  std::string path = Environment::getDataDir() + "/fonts/" + name + ".txt";
  Font font = loadFont(path, name);
  auto inserted = fontCache.insert(std::pair<std::string, Font>(name, font));
  // Return reference to newly inserted Font
  return inserted.first->second;
}

Font FontLoader::loadFont(const std::string &path, const std::string &name) {
  Font font;
  std::ifstream input(path);
  for (std::string line; getline(input, line);) {
    std::stringstream stream(line);

    if (line.length() > 0) {
      std::string tokens[9];

      for (int i = 0; i < 9; i++) {
        stream >> tokens[i];
      }

      if (tokens[0] == "char") {
        //Delete all the letters from the tokens
        for (int i = 1; i < 9; ++i) {
          //Find the start of the digits
          int pos = tokens[i].find('=', 0) + 1;
          //Erase the rest
          tokens[i].erase(0, pos);
        }

        Glyph letter;
        int id = std::stoi(tokens[1]);
        letter.x = std::stoi(tokens[2]);
        letter.y = std::stoi(tokens[3]);
        letter.width = std::stoi(tokens[4]);
        letter.height = std::stoi(tokens[5]);
        letter.xOffset = std::stof(tokens[6]);
        letter.yOffset = std::stof(tokens[7]);
        letter.advance = std::stof(tokens[8]);

        //Load the mesh
        Texture texture = TextureLoader::getTexture(name + ".png");
        letter.mesh = MeshLoader::getSubPlane(letter.x, letter.y, letter.width, letter.height,
          texture.width, texture.height);

        font.letters.insert(std::pair<int, Glyph>(id, letter));
      }
    }
  }

  return font;
}

} /* namespace glPortal */
