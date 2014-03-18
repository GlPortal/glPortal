#include "Environment.hpp"

using namespace std;
using namespace glPortal::util;

namespace glPortal {
  namespace engine{
    void Environment::init(){
    this->config = ConfigFileParser("./data/main.cfg");
    }
    
    ConfigFileParser Environment::getConfig(){
      return this->config;
    }
  }
}
