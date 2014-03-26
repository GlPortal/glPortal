#include "Main.hpp"
#include "engine/Resources.hpp"
#include <iostream>
#include "engine/Box.hpp"
#include "engine/Environment.hpp"

using namespace glPortal::map;
using namespace glPortal::engine;
using namespace glPortal::util::audioplayer;
using namespace std;

int main(int argc, char **argv) {
  Environment::init();
  window.setup(&argc, argv);
  setup(&argc, argv);
  loop();

  return EXIT_SUCCESS;
}

void setup(int *argc, char **argv) {
  height = window.getHeight();
  width = window.getWidth();
  window.enableGlFeatures();
  game.loadTextures();
  game.unpause();
}

void loop() {
  SDL_Event event;
  bool quit = false;

  AudioPlayer* audioPlayer = new AudioPlayer();
  audioPlayer->init();
  audioPlayer->playByFileName("data/audio/music/track1.ogg");
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
    draw();
  }
  window.close();
  audioPlayer->cleanUp();
}

void draw() {
  game.setWindow(window);
  game.setHeightWidth(height, width);
  game.draw();
  window.swapBuffer();
}
