#ifndef CONFIGFILEPARSER_HPP
#define CONFIGFILEPARSER_HPP

#include <map>
#include <string>

namespace glPortal {

class ConfigFileParser {
private:
  std::map<std::string, std::string> configMap;
public:
  ConfigFileParser(const std::string &filename);
  std::string getStringByKey(const std::string &key);
  int getIntByKey(const std::string &key);
  float getFloatByKey(const std::string &key);
};

}

#endif /* CONFIGFILEPARSER_HPP */
