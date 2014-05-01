#ifndef __ENVIRONMENT_HPP
#define __ENVIRONMENT_HPP

#include <vector>
#include <string>
#include <cstring>
#include <cstdlib> 
#include <map>
#include "../util/ConfigFileParser.hpp"

using namespace std;
using namespace glPortal::util;

namespace glPortal {
  namespace engine{
    class Environment{
    private:
      static ConfigFileParser *config;
      static std::string * datadir;
    public:
      static const float FRAMETIME_SECONDS;
      static void init(int argc, char **argv);
      static ConfigFileParser & getConfig();
      static ConfigFileParser * getConfigPointer();
      static void initializeConfig();
      static std::string getDataDir();
    };
  }
}

#endif
