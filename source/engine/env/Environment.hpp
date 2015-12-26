#ifndef ENVIRONMENT_HPP
#define ENVIRONMENT_HPP

#include <string>

#include <engine/core/event/Dispatcher.hpp>
#include <engine/env/Config.hpp>

namespace glPortal {

class Environment {
private:
  static Config config;
  static std::string datadir;

public:
  static Dispatcher dispatcher;
  static void init();
  static Config& getConfig();
  static void initializeConfig();
  static std::string getDataDir();
  static void setDataDir(const std::string &string);
};

} /* namespace glPortal */

#endif /* ENVIRONMENT_HPP */
