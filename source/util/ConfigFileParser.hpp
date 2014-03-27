#ifndef __CONFIGFILEPARSER_HPP
#define __CONFIGFILEPARSER_HPP

#include <vector>
#include <string>
#include <cstring>
#include <cstdlib> 
#include <map>

using namespace std;

namespace glPortal {
  namespace util{
    class ConfigFileParser{
    private:
      std::map<string,string> configMap;
    public:
      ConfigFileParser(std::string filename);
      std::string getConfigValueByKey(std::string key);
      int getConfigIntValueByKey(std::string key);
    };
  }
}

#endif
