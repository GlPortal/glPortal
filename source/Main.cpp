#include "Game.hpp"

#include <stdexcept>
#include <string>

#include <radix/env/Environment.hpp>
#include <radix/env/ArgumentsParser.hpp>
#include <radix/env/Util.hpp>
#include <radix/core/diag/Throwables.hpp>
#include <radix/env/Config.hpp>

using namespace glPortal;

int main(const int argc, char *argv[]) {
  radix::Util::Init();
  radix::ArgumentsParser::setEnvironmentFromArgs(argc, argv);
  radix::Environment::init();
  radix::Config &config = radix::Environment::getConfig();
  radix::ArgumentsParser::populateConfig(config);

  try {
    Game game(config);
    while (game.isRunning()) {
      game.processInput();
      game.update();
      game.render();
    }
    game.cleanUp();
  } catch (radix::Exception::Error &err) {
    radix::Util::Log(radix::Error, err.source()) << err.what();
  }

  return 0;
}
