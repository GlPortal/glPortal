#include "Environment.hpp"
#include <stdexcept>

using namespace std;

namespace glPortal {
  namespace engine{
    
    std::string * Environment::datadir = 0;
    ConfigFileParser *Environment::config = NULL;
    
    void Environment::init(int argc, char **argv){
      if (!datadir)
        datadir = new std::string;
      for (int i = 0; i < argc; ++ i ) {
        if ((std::string(argv[i])== "-datadir") && (i+1<argc)) {
          *datadir = std::string(argv[i + 1]);
        }
      }
      // default installation dir
      if (datadir->empty()) {
        *datadir = "/usr/share/glportal/data";
      }
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
        config = new ConfigFileParser(getDataDir() + "/private.cfg");
      } catch (const std::invalid_argument& e){
        config = new ConfigFileParser(getDataDir() + "/main.cfg");
      }
    }
    
    std::string Environment::getDataDir()
    {
      if (!datadir)
        datadir = new std::string;
      return *datadir;
    }

  }
}

