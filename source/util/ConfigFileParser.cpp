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
      if(!file){
	throw std::invalid_argument("File " + filename + " not found.");
      } else {

	while(file.good()) {
	  std::getline(file, line);
	  std::stringstream stringStream(line);
	  if(line.length() > 0) {
	    vector<std::string> strings;
	    tokenizer<> tok(line);
	    for(tokenizer<>::iterator beg=tok.begin(); beg!=tok.end();++beg){
	      strings.push_back(*beg);
	      
	    }
	    configMap[strings.at(0)]=strings.at(1);
	  }
	}
	file.close();
      }
    }
    
    std::string ConfigFileParser::getConfigValueByKey(std::string key){
      try{
	return configMap.at(key);
      } catch (const std::out_of_range& e){
	throw std::invalid_argument("No such value for key.");
      }
    }

    int ConfigFileParser::getConfigIntValueByKey(std::string key){
      return atoi(getConfigValueByKey(key).c_str());
    }
  }
}
