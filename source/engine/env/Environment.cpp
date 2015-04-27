#include "Environment.hpp"

#include <stdexcept>
#include <getopt.h>
#include "ConfigFileParser.hpp"
#include <engine/core/file/Path.hpp>
#include <iostream>

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

  int argument;

  static struct option long_options[] = {
    {"version",  no_argument,       0, 'v'},
    {"help",     no_argument,       0, 'h'},
    {"datadir",  required_argument, 0, 'd'},
    {0, 0, 0, 0}
  };
  
  while (1){
    int option_index = 0;
    argument = getopt_long (argc, argv, "v:d:h:",long_options, &option_index);

    if (argument == -1)
      break;
    /// Command Line arguments
    switch (argument) {
    case 'v':
      /// - version \n
      /// Display the current version.
      std::cout << "Current version\n";
      exit(0);
    case 'd':
      /// - datadir \n
      /// Set directory where the game data is stored.
      *datadir = std::string(optarg);
      break;
    case 'h':
      /// - help \n
      /// Display the help.
      std::cout << "Help\n";
      exit(0);

    default:
      break;
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
