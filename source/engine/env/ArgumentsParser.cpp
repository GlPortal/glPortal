#include "ArgumentsParser.hpp"
#include "Environment.hpp"

#include <cstdlib>
#include <stdexcept>
#include <string>
#include <getopt.h>
#include <iostream>

#include <engine/core/file/Path.hpp>
#include "engine/env/Config.hpp"

namespace glPortal {
std::string ArgumentsParser::mapName = "";
std::string ArgumentsParser::mapPath = "";
void ArgumentsParser::setEnvironmentFromArgs(int argc, char **argv) {
  int argument;
  
  static struct option long_options[] = {
    {"version",          no_argument,       0, 'v'},
    {"help",             no_argument,       0, 'h'},
    {"datadir",          required_argument, 0, 'd'},
    {"map",              required_argument, 0, 'm'},
    {"mapfrompath",      required_argument, 0, 'p'},
    {0, 0, 0, 0}
  };
  
  while (1) {
    int option_index = 0;
    argument = getopt_long (argc, argv, "v:d:h:m:p", long_options, &option_index);

    if (argument == -1) {
      break;
    }
    
    /// Command Line arguments
    switch (argument) {
    case 'v':
      /// - version \n
      /// Display the current version.
      std::cout << "GlPortal Version 0.1\n";
      exit(0);
    case 'd':
      /// - datadir \n
      /// Set directory where the game data is stored.
      Environment::setDataDir(optarg);
      break;
    case 'h':
      /// - help \n
      /// Display the help.
      std::cout << "Help\n";
      exit(0);
    case 'm':
      /// - map \n
      /// Set the map that should be loaded.
      mapName = optarg;
    case 'p':
      /// - mapFromPath \n
      /// Set the map that should be loaded.
      mapPath = optarg;
    default:
      break;
    }
  }
}
void ArgumentsParser::populateConfig() {
  Config &config = Environment::getConfig();
  if (mapName != ""){
    config.map = mapName;
  }
  if (mapPath != ""){
    config.mapPath = mapPath;
  }  
}
} /* namespace glPortal */
