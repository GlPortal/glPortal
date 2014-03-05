#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>
#include "StringConverter.hpp"


namespace glPortal {
  namespace engine {
    float StringConverter::stringStreamToFloat(std::stringstream& stream){
      std::string string;
      stream >> string;

      return static_cast<float>(atof(string.c_str()));
    }

    std::string StringConverter::stringStreamToString(std::stringstream& stream){
      std::string string;
      stream >> string;
      
      return string;
    }

  }
}
