#ifndef ENVIRONMENT_HPP
#define ENVIRONMENT_HPP

#include <string>

using namespace std;

namespace glPortal {
class ConfigFileParser;

class Environment {
private:
  static ConfigFileParser*config;
  static std::string datadir;
public:
  static void init();
  static ConfigFileParser& getConfig();
  static ConfigFileParser* getConfigPointer();
  static void initializeConfig();
  static std::string getDataDir();
  static void setDataDir(const std::string &dir);
};

} /* namespace glPortal */

#endif /* ENVIRONMENT_HPP */
