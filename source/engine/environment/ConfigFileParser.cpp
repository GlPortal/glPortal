#include "ConfigFileParser.hpp"
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <cstdlib>
#include <stdexcept>      
using namespace std;

namespace glPortal {
  ConfigFileParser::ConfigFileParser(std::string filename) {
    std::ifstream file(filename, std::ifstream::in);
    std::string line, string, stringBuffer;
    if(!file.is_open()) {
      throw std::invalid_argument("File " + filename + " not found.");
    } else {
      while(file.good()) {
        std::getline(file, line);
        std::stringstream stringStream(line);
        int i;
        if(line.length() > 0) {
          std::string seperator = " ";
          vector<std::string> strings;
          for(i=0;i<line.length();i++){
            if(line[i] == ' '){
              strings.push_back(stringBuffer);
              stringBuffer = "";
            } else {
              std::string character(1, line[i]);
              const char* characterConstant = character.c_str();
              stringBuffer.append(characterConstant);
            }
          }
          strings.push_back(stringBuffer);
          configMap[strings.at(0)] = strings.at(1);
        }
      }
      file.close();
    }
  }
  
  std::string ConfigFileParser::getStringByKey(std::string key) {
    try{
      return configMap.at(key);
    } catch (const std::out_of_range& e) {
      throw std::invalid_argument("No such value for key.");
    }
  }
  
  int ConfigFileParser::getIntByKey(std::string key) {
    return atoi(getStringByKey(key).c_str());
  }
  
  float ConfigFileParser::getFloatByKey(std::string key) {
    float s = atof(getStringByKey(key).c_str());
    return s;
  }
}

