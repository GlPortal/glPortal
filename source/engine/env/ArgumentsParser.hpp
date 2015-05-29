#ifndef ARGUMENTS_PARSER_HPP
#define ARGUMENTS_PARSER_HPP

#include <string>

using namespace std;

namespace glPortal {

class ArgumentsParser {
private:
  static std::string mapName;
public:
  static void setEnvironmentFromArgs(int argc, char **argv);
  static void populateConfig();
};

} /* namespace glPortal */

#endif /* ARGUMENTS_PARSER_HPP */
