#include "Environment.hpp"

#include <stdexcept>
#include <getopt.h>
#include <engine/core/file/Path.hpp>
#include <iostream>

namespace glPortal {

Dispatcher Environment::dispatcher;
Config Environment::config;

std::string Environment::datadir = "";

/** @class Environment
    @brief Manager for environment and config

    Manages environment variables and configuration data. Get an instance of ConfigFileParser.
*/

void Environment::init() {
  // default installation dir
  if (datadir.empty()) {
#ifndef _WIN32
    datadir = "/usr/share/glportal/data";
#else
    datadir = "data";
#endif
  }
  initializeConfig();
}

Config& Environment::getConfig() {
  return config;
}

void Environment::initializeConfig() {
  config.load();
}

std::string Environment::getDataDir() {
  return datadir;
}

void Environment::setDataDir(const std::string &string) {
  datadir = string;
}

} /* namespace glPortal */
