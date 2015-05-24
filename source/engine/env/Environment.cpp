#include "Environment.hpp"

#include <stdexcept>
#include <getopt.h>
#include "ConfigFileParser.hpp"
#include <engine/core/file/Path.hpp>
#include <iostream>

namespace glPortal {  
std::string Environment::datadir = "";
ConfigFileParser *Environment::config = NULL;

/** @class Environment
    @brief Manager for environment and config

    Manages environment variables and configuration data. Get an instance of ConfigFileParser.
*/
  
void Environment::init() {
  int argument;

  static struct option long_options[] = {
    {"version",  no_argument,       0, 'v'},
    {"help",     no_argument,       0, 'h'},
    {"datadir",  required_argument, 0, 'd'},
    {0, 0, 0, 0}
  };
  

  // default installation dir
  if (datadir.empty()) {
#ifndef _WIN32
    datadir = "/usr/share/glportal/data/";
#else
    datadir = "data/";
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
  return datadir;
}

void Environment::setDataDir(const std::string &dir) {
  datadir = dir;
}
  
} /* namespace glPortal */
