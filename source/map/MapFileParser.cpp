#include "MapFileParser.hpp"
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <cstdlib>
#include "../GameMap.hpp"
#include "../engine/Resources.hpp"
#include "../engine/StringConverter.hpp"
#include "../Exception.hpp"

namespace glPortal {
  namespace map{

    /**
     * Get a gameMapfrom a gameMapfile.
     *
     * @param filename Path to the .gameMapfile
     */
    GameMap MapFileParser::getMapFromFile(const char *filename) {
      std::ifstream file(filename, std::ifstream::in);
      std::string line, string;

      this->gameMap.flush();

      // Read gameMapfile line by line
      while(file.good()) {
	std::getline(file, line);
	std::stringstream stringStream(line);
	if(line.length() > 0) {
	  stringStream >> string;
	  float values[6];
	  switch(line[0]) {
	    // Wall definition
	  case 'w':
	    for(int i = 0; i < 6; i++) {
	      values[i] = StringConverter::stringStreamToFloat(stringStream);
	    }
	    this->gameMap.addWallBox(Box(values, TID_WALL));
	    break;
	    // Tiles definition
	  case 't':
	    for(int i = 0; i < 6; i++) {
	      values[i] = StringConverter::stringStreamToFloat(stringStream);
	    }
	    this->gameMap.addWallBox(Box(values, TID_TILES));
	    break;
	    // Acid pool definition
	  case 'a':
	    for(int i = 0; i < 6; i++) {
	      values[i] = StringConverter::stringStreamToFloat(stringStream);
	    }
	    this->gameMap.addAcidBox(Box(values, TID_ACID));
	    break;
	    // Light position
	  case 'l':
	    for(int i = 0; i < 3; i++) {
	      lightpos[i] = StringConverter::stringStreamToFloat(stringStream);
	    }
	    lightpos[3] = 1.f; // Set as positioned light
	    
	    this->gameMap.setLightPosition(lightpos);
	    break;
	    // Start position
	  case 's':
	    for(int i = 0; i < 3; i++) {
	      startpos[i] = StringConverter::stringStreamToFloat(stringStream);
	    }
	    this->gameMap.setSpawnPosition(startpos);
	    break;
	    // Cake/goal position
	  case 'c':
	    for(int i = 0; i < 3; i++) {
	      cakepos[i] = StringConverter::stringStreamToFloat(stringStream);
	    }
	    this->gameMap.setCakePosition(cakepos);
	    break;
	  }
	}
      }
      file.close();
      return this->gameMap;
    }
  }
}
