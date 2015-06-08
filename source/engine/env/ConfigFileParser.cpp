#include "ConfigFileParser.hpp"

#include <cstdlib>
#include <fstream>
#include <stdexcept>
#include <vector>
#include <sstream>
#include <iostream>

using namespace std;

namespace glPortal {

/** @class ConfigFileParser
    @brief Parse config files

    Parses config values from a file. Key and value are seperated by a whitespace. 
    Each entry is on a new line.
*/

ConfigFileParser::ConfigFileParser(const std::string &filename) {
  std::ifstream file(filename, std::ifstream::in);
  std::string line, stringBuffer;
  if (!file.is_open()) {
    throw std::invalid_argument("File " + filename + " not found.");
  } else {
    while (file.good()) {
      std::getline(file, line);
      std::stringstream stringStream(line);

      if (line.length() > 0) {
        vector<std::string> strings;
        for (unsigned int i = 0; i < line.length(); i++) {
          if (line[i] == ' ') {
            strings.push_back(stringBuffer);
            stringBuffer = "";
          } else {
            std::string character(1, line[i]);
            const char* characterConstant = character.c_str();
            stringBuffer.append(characterConstant);
          }
        }
        strings.push_back(stringBuffer);
        stringBuffer = "";
        if(strings.size() >= 2){
          configMap[strings.at(0)] = strings.at(1);
        } else {
          throw std::runtime_error("Config parser is expecting two items per line.");
        }
      }
    }
    file.close();
  }
}

std::string ConfigFileParser::getStringByKey(const std::string &key) {
  try {
    return configMap.at(key);
  } catch (const std::out_of_range& e) {
    throw std::invalid_argument("No such value for key.");
  }
}

int ConfigFileParser::getIntByKey(const std::string &key) {
  return atoi(getStringByKey(key).c_str());
}

float ConfigFileParser::getFloatByKey(const std::string &key) {
  float s = atof(getStringByKey(key).c_str());
  return s;
}

void ConfigFileParser::setStringByKey(const std::string &key, const std::string &value) {
  //configMap.erase(configMap.find(key));
  //configMap.insert ( std::pair<std::string, std::string>(key, value) );
  // configMap.at(key) = value;
  configMap[key] = value;
}
  
} /* namespace glPortal */

