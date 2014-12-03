#include "Environment.hpp"

#include <stdexcept>

#include "ConfigFileParser.hpp"
#include <engine/core/file/Path.hpp>

using namespace std;

namespace glPortal {  
std::string * Environment::datadir = 0;
ConfigFileParser *Environment::config = NULL;

/** @class Environment
    @brief Manager for environment and config

    Manages environment variables and configuration data. Get an instance of ConfigFileParser.
*/
  
void Environment::init(int argc, char **argv) {
  if (!datadir)
    datadir = new std::string;
  for (int i = 0; i < argc; ++i) {
    if ((std::string(argv[i]) == "-datadir") && (i + 1 < argc)) {
      *datadir = std::string(argv[i + 1]);
    }
  }

  // default installation dir
  if (datadir->empty()) {
#ifndef _WIN32
    *datadir = "/usr/share/glportal/data/";
#else
    *datadir = "data/";
#endif
  }
  initializeConfig();
}

ConfigFileParser & Environment::getConfig() {
  if (!config) {
    initializeConfig();
  }

  return *config;
}

ConfigFileParser * Environment::getConfigPointer() {
  if (!config) {
    initializeConfig();
  }

  return config;
}

void Environment::initializeConfig() {
  try {
    config = new ConfigFileParser(
        Path::FromUnixPath(getDataDir() + "/private.cfg"));
  } catch (const std::invalid_argument& e) {
    config = new ConfigFileParser(
        Path::FromUnixPath(getDataDir() + "/main.cfg"));
  }
}

std::string Environment::getDataDir() {
  if (!datadir)
    datadir = new std::string;
  return *datadir;
}

} /* namespace glPortal */
