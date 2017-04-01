#include <glPortal/Game.hpp>

#include <radix/env/Environment.hpp>
#include <radix/env/ArgumentsParser.hpp>
#include <radix/core/diag/Throwables.hpp>
#include <linenoise.hpp>
#include <iostream>

#undef main
using namespace std;
using namespace glPortal;

int main(const int argc, char *argv[]) {
    const auto path = "history.txt";

    // Enable the multi-line mode
    linenoise::SetMultiLine(true);

    // Set max length of the history
    linenoise::SetHistoryMaxLen(4);

    // Setup completion words every time when a user types
    linenoise::SetCompletionCallback([](const char* editBuffer, std::vector<std::string>& completions) {
        if (editBuffer[0] == 'h') {
            completions.push_back("hello");
            completions.push_back("hello there");
        }

        if (editBuffer[0] == 'q') {
          completions.push_back("quit");
        }
    });

    // Load history
    linenoise::LoadHistory(path);

    while (true) {
        std::string line;

        auto quit = linenoise::Readline("command > ", line);

        if (quit) {
          break;
        }

        if (line == "quit") {
          cout <<  "Goodbye" << endl;
          break;
        }

        cout <<  "echo: '" << line << "'" << endl;

        // Add line to history
        linenoise::AddHistory(line.c_str());

        // Save history
        linenoise::SaveHistory(path);
    }

    return 0;



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
