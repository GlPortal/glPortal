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

  const std::string& getString(const std::string &key);
  void setString(const std::string &key, const std::string &value);

  int getInt(const std::string &key);

  float getFloat(const std::string &key);

  bool getBool(const std::string &key);
};

}

#endif /* CONFIGFILEPARSER_HPP */
