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
  game.setPlayer(player);
  game.respawn();
}

void loop() {
  SDL_Event event;
  bool quit = false;

  AudioPlayer* player = new AudioPlayer();
  player->init();
  player->playByFileName("data/audio/music/track1.ogg");
  player->play();    
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
        resize(event.window.data1, event.window.data2);
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
  player->cleanUp();
}

void respawn() {
  game.resetFade();
  player.create(gameMap.getStartX(), gameMap.getStartY(), gameMap.getStartZ());
}

void draw() {
  game.setWindow(window);
  game.setHeightWidth(height, width);
  game.draw();
  window.swapBuffer();
}

void resize(int w, int h) {
  window.setSize(w, h);
}

