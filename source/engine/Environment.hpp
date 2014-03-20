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
      ConfigFileParser *config;
    public:
      Environment();
      void init();
      ConfigFileParser & getConfig();
    };
  }
}

#endif
