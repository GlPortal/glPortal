#ifndef CONFIGFILEPARSER_HPP
#define CONFIGFILEPARSER_HPP

#include <map>
#include <string>

using namespace std;

namespace glPortal {

class ConfigFileParser {
private:
  std::map<string, string> configMap;
public:
  ConfigFileParser(std::string filename);
  std::string getStringByKey(std::string key);
  int getIntByKey(std::string key);
  float getFloatByKey(std::string key);
  void setStringByKey(std::string key, std::string value);
};

}

#endif /* CONFIGFILEPARSER_HPP */
