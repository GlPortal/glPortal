#include "Game.hpp"

#include <stdexcept>

#include <radix/core/diag/Throwables.hpp>
#include <radix/env/Environment.hpp>
#include <radix/env/ArgumentsParser.hpp>
#include <radix/env/Util.hpp>

using namespace glPortal;

int main(const int argc, char *argv[]) {
  try {
    radix::Util::Init();
    radix::ArgumentsParser::setEnvironmentFromArgs(argc, argv);
    Game game;
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
