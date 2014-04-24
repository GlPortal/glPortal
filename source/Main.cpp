#include "Main.hpp"
#include "engine/Resources.hpp"
#include <iostream>
#include "engine/Box.hpp"
#include "engine/Environment.hpp"
#include "Path.hpp"

using namespace glPortal::map;
using namespace glPortal::engine;
using namespace glPortal::util::audioplayer;
using namespace std;

int main(int argc, char * argv[]) {
  Environment::init(argc, argv);
  
  Player player;
  GameMap gameMap;
  Game game;
  Window window;

  window.setup(argc, argv);

  window.enableGlFeatures();
  game.loadTextures();
  
  loop(game, window);

  return EXIT_SUCCESS;
}



void loop(Game & game, Window & window) {
  SDL_Event event;
  bool quit = false;
  int height = window.getHeight();
  int width = window.getWidth();
  AudioPlayer* audioPlayer = new AudioPlayer();
  audioPlayer->init();
  string audioFile(Path::FromUnixPath(Environment::getDataDir()+"/audio/music/track1.ogg"));
  audioPlayer->playByFileName(audioFile);
  audioPlayer->play();    
  while (!quit) {
    while(SDL_PollEvent(&event)) {
      if(event.type == SDL_QUIT) {
        quit = true;
      }
      if(event.type == SDL_KEYDOWN) {
	      game.setKey(event.key.keysym);
      }
      if(event.type == SDL_KEYUP) {
	      game.unsetKey(event.key.keysym);
      }
      if(event.type == SDL_MOUSEBUTTONDOWN) {
	      game.mousePressed(event.button.button);
      }
      if(event.type == SDL_WINDOWEVENT_RESIZED) {
	      window.setSize(event.window.data1, event.window.data2);
      }
      if(event.type == SDL_WINDOWEVENT_FOCUS_LOST) {
        game.pause();
      }
      if(event.type == SDL_WINDOWEVENT_FOCUS_GAINED) {
        game.unpause();
      }
    }
    game.update();
    game.setWindow(window);
    game.setHeightWidth(height, width);
    game.draw();
    window.swapBuffer();
  }
  window.close();
  audioPlayer->cleanUp();
}

