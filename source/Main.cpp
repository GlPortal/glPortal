#include <glPortal/Game.hpp>

#include <radix/env/Environment.hpp>
#include <radix/env/ArgumentsParser.hpp>
#include <radix/core/diag/Throwables.hpp>
#include <iostream>

#undef main
using namespace std;
using namespace glPortal;

int main(const int argc, char *argv[]) {
  radix::Util::Init();
  radix::ArgumentsParser::setEnvironmentFromArgs(argc, argv);
  try {
    Game game;
    game.setup();
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
