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
    public:
      static void init();
      static ConfigFileParser & getConfig();
      static void initializeConfig();
    };
  }
}

#endif
