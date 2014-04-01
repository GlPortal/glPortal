#include "ConfigFileParser.hpp"
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <cstdlib>
#include "../engine/StringConverter.hpp"
#include <boost/foreach.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/tokenizer.hpp>
#include <stdexcept>      
using namespace boost;
using namespace std;
using namespace glPortal::engine;

namespace glPortal {
  namespace util{
    ConfigFileParser::ConfigFileParser(std::string filename) {
      std::ifstream file(filename, std::ifstream::in);
      std::string line, string;
      if(!file) {
	throw std::invalid_argument("File " + filename + " not found.");
      } else {
	while(file.good()) {
	  std::getline(file, line);
	  std::stringstream stringStream(line);
	  if(line.length() > 0) {
	    vector<std::string> strings;
	          
	    boost::char_separator<char> sep(" ");
	    tokenizer<char_separator<char>> tok(line, sep);
	    for(tokenizer<char_separator<char>>::iterator beg = tok.begin(); beg != tok.end(); ++beg) {
	      std::string sis;
	      sis = *beg;
	      strings.push_back(*beg);
	    }
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
}

