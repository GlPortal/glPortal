#include "ConfigFileParser.hpp"

#include <cstdlib>
#include <fstream>
#include <stdexcept>
#include <vector>
#include <sstream>
#include <iostream>

using namespace std;

namespace glPortal {

ConfigFileParser::KeyNotFound::KeyNotFound(const string &key)
  : out_of_range("Key not found in configuration"), key(key) {
}

const char* ConfigFileParser::KeyNotFound::which() const {
  return key.c_str();
}

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
        for (unsigned int i = 0; i < line.length(); ++i) {
          if (line[i] == ' ') {
            strings.push_back(stringBuffer);
            stringBuffer = "";
          } else {
            stringBuffer += line[i];
          }
        }
        strings.push_back(stringBuffer);
        stringBuffer = "";
        if(strings.size() >= 2) {
          configMap[strings.at(0)] = strings.at(1);
        } else {
          throw std::runtime_error("Config parser is expecting two items per line.");
        }
      }
    }
    file.close();
  }
}

const string& ConfigFileParser::getString(const string &key) {
  try {
    return configMap.at(key);
  } catch (const std::out_of_range&) {
    throw KeyNotFound(key);
  }
}

void ConfigFileParser::setString(const string &key, const string &value) {
  configMap[key] = value;
}

int ConfigFileParser::getInt(const string &key) {
  return atoi(getString(key).c_str());
}

float ConfigFileParser::getFloat(const string &key) {
  float s = atof(getString(key).c_str());
  return s;
}

bool ConfigFileParser::getBool(const string &key) {
  const string &b = getString(key);
  if (b == "yes" || b == "true" || b == "1" || b == "on") {
    return true;
  }
  return false;
}
  
} /* namespace glPortal */

