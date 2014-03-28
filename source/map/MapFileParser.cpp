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
#include "../util/Vector3f.hpp"

namespace glPortal {
  namespace map{

    /**
     * Get a map from a map file.
     *
     * @param filename Path to the map file
     */
    GameMap MapFileParser::getMapFromFile(std::string filename) {
      std::ifstream file(filename, std::ifstream::in);
      std::string line, string;

      this->gameMap.flush();

      if(!file){
	cout << "Unable to load map.";
	this->gameMap.setIsLastScreen();
	return this->gameMap;
      }

      // Read map file line by line
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
	  case 'b':{
	    for(int i = 0; i < 3; i++) {
	      barrelPosition[i] = StringConverter::stringStreamToFloat(stringStream);
	    }
	    barrelPosition[3] = 1.f; // Set as positioned light
	    Model* barrelModel = new Model("data/objects/plastic_barrel/small_barrel.obj");
	    barrelModel->setPosition(0, 0, 0);

	    this->gameMap.addObject(*barrelModel);
	    break;
	  }
	    // Start position
	  case 's':
	    for(int i = 0; i < 3; i++) {
	      startpos[i] = StringConverter::stringStreamToFloat(stringStream);
	    }
	    this->gameMap.setSpawnPosition(startpos[0], startpos[1], startpos[2]);
	    break;
	    // Cake/goal position
	  case 'c':
	    for(int i = 0; i < 3; i++) {
	      cakepos[i] = StringConverter::stringStreamToFloat(stringStream);
	    }
	    this->gameMap.setEndPosition(cakepos[0], cakepos[1], cakepos[2]);
	    break;
	  case 'o':
	    std::string optionLine;
	    optionLine = StringConverter::stringStreamToString(stringStream);
	    std::string jetpackOptionIndicator("j");

	    if (optionLine.find(jetpackOptionIndicator) != std::string::npos) {
	      this->gameMap.enableJetpack();
	    }
	    break;
	  }
	}
      }
      file.close();
      return this->gameMap;
    }
  }
}
