#include "ArgumentsParser.hpp"
#include "Environment.hpp"

#include <stdexcept>
#include <getopt.h>
#include <engine/core/file/Path.hpp>
#include <iostream>

namespace glPortal {  

void ArgumentsParser::setEnvironmentFromArgs(int argc, char **argv) {
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
      Environment::setDataDir(optarg);
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
}
  
} /* namespace glPortal */
