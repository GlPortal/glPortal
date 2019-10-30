#include <glPortal/Game.hpp>

#include <radix/env/LegacyEnvironment.hpp>
#include <radix/env/ArgumentsParser.hpp>
#include <radix/core/diag/Throwables.hpp>

#undef main

using namespace glPortal;

int main(const int argc, char *argv[]) {
  radix::Util::Init();
  radix::LegacyEnvironment::Init();
  radix::ArgumentsParser::setEnvironmentFromArgs(argc, argv);
  try {
    Game game;
    game.setup();
    while (game.isRunning()) {
      game.preCycle();
      game.processInput();
      game.update();
      game.render();
      game.postCycle();
    }
    game.cleanUp();
  } catch (radix::Exception::Error &err) {
    radix::Util::Log(radix::Error, err.source()) << err.what();
  }

  return 0;
}
