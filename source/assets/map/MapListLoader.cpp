#include "MapListLoader.hpp"

#include <fstream>
#include <iostream>
#include <stdexcept>

#include <engine/env/Environment.hpp>

using namespace std;

namespace glPortal {

vector<string> MapListLoader::getMapList() {
  vector<string> mapList;
  std::string path = Environment::getDataDir() + "/maps/maplist";
  ifstream file(path);

  if (not file.is_open()) {
    throw runtime_error("Could not find file: " + path);
  }

  string line;

  while (std::getline(file, line)) {
    mapList.push_back(line);
  }

  return mapList;
}

} /* namespace glPortal */
