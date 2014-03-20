#include "Environment.hpp"

using namespace std;
//using namespace glPortal::util;

namespace glPortal {
  namespace engine{
    Environment::Environment(){
      //Do nothing
    }
    
    void Environment::init(){
      this->config = new ConfigFileParser("./data/main.cfg");
    }
    
    ConfigFileParser & Environment::getConfig(){
      return *this->config;
    }
  }
}
