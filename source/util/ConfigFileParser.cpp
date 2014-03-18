#include "ConfigFileParser.hpp"
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <cstdlib>
#include "../engine/StringConverter.hpp"
#include <boost/foreach.hpp>
#include <boost/algorithm/string.hpp>

using namespace std;
using namespace glPortal::engine;

namespace glPortal {
  namespace util{
    ConfigFileParser::ConfigFileParser(std::string filename) {
      std::ifstream file(filename, std::ifstream::in);
      std::string line, string;
      if(!file){

      } else {

	while(file.good()) {
	  std::getline(file, line);
	  std::stringstream stringStream(line);
	  if(line.length() > 0) {
	    // tokenize string and write to map like this
	    //	    configMap[pair[0]]=pair[1];
	  }
	}
	file.close();
      }
    }

    std::string ConfigFileParser::getConfigValueByKey(std::string key){

      return configMap.at(key);
    }
  }
}
