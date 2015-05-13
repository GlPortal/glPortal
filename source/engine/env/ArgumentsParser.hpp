#ifndef ARGUMENTS_PARSER_HPP
#define ARGUMENTS_PARSER_HPP

#include <string>

using namespace std;

namespace glPortal {

class ArgumentsParser {
public:
  static void setEnvironmentFromArgs(int argc, char **argv);
};

} /* namespace glPortal */

#endif /* ARGUMENTS_PARSER_HPP */
