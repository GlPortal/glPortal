#ifndef __CONFIGFILEPARSER_HPP
#define __CONFIGFILEPARSER_HPP

#include <vector>
#include <string>
#include <cstring>
#include <cstdlib> 
#include <map>

using namespace std;

namespace glPortal {
  class ConfigFileParser{
  private:
    std::map<string,string> configMap;
  public:
    ConfigFileParser(std::string filename);
    std::string getStringByKey(std::string key);
    int getIntByKey(std::string key);
    float getFloatByKey(std::string key);
  };
}

#endif
