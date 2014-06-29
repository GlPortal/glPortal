#ifndef ENVIRONMENT_HPP
#define ENVIRONMENT_HPP

#include <string>

using namespace std;

namespace glPortal {
class ConfigFileParser;

class Environment {
private:
  static ConfigFileParser*config;
  static std::string* datadir;
public:
  static void init(int argc, char **argv);
  static ConfigFileParser& getConfig();
  static ConfigFileParser* getConfigPointer();
  static void initializeConfig();
  static std::string getDataDir();
};

} /* namespace glPortal */

#endif /* ENVIRONMENT_HPP */
