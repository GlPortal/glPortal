#include "Environment.hpp"
#include <stdexcept>

using namespace std;

namespace glPortal {
  namespace engine{
    const float Environment::FRAME_TIME = 17;
    ConfigFileParser *Environment::config = NULL;
    
    void Environment::init(){
      initializeConfig();
    }

    ConfigFileParser & Environment::getConfig(){
      if(!config){
	initializeConfig();
      }
      
      return *config;
    }

    ConfigFileParser * Environment::getConfigPointer(){
      if(!config){
	initializeConfig();
      }
      
      return config;
    }

    
    void Environment::initializeConfig(){
      try{
	config = new ConfigFileParser("./data/private.cfg");
      } catch (const std::invalid_argument& e){
	config = new ConfigFileParser("./data/main.cfg");
      }
    }
  }
}

