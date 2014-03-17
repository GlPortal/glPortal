#include "ListFileParser.hpp"
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <cstdlib>
#include "../engine/StringConverter.hpp"

using namespace std;
using namespace glPortal::engine;

namespace glPortal {
  namespace util{
    vector<string> ListFileParser::getListFromFile(const char *filename) {
      std::ifstream file(filename, std::ifstream::in);
      std::string line, string;
      this->list.clear();
      if(!file){
	return this->list;
      }

      while(file.good()) {
	std::getline(file, line);
	std::stringstream stringStream(line);
	if(line.length() > 0) {
	  stringStream >> string;
	  this->list.push_back(string);
	}
      }
      file.close();
      return this->list;
    }
  }
}
