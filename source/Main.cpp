#include "Main.hpp"
#include "engine/Resources.hpp"
#include <iostream>
#include "engine/Box.hpp"

using namespace glPortal::map;
using namespace glPortal::engine;
using namespace std;

int main(int argc, char **argv) {
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
  nmap_enabled = true;
  game.setPlayer(player);
  game.respawn();
}

void update(int value) {
  if(!game.isPaused()) {
    game.update();
    
    if(player.isDead()) {
      game.fadeOut();
    }
  }
}

void loop() {
  SDL_Event event;
  bool quit = false;
    
  while (!quit) {
    while (SDL_PollEvent(&event)) {
      if(event.type == SDL_QUIT) {
        quit = true;
      }
      if(event.type == SDL_KEYDOWN) {
        key_down(event.key);
      }
      if(event.type == SDL_KEYUP) {
        key_up(event.key);
      }
      if(event.type == SDL_MOUSEBUTTONDOWN) {
        mouse_pressed(event.button);
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
    update(FRAMETIME);
    draw();
  }
  window.close();
}

void resetKeyStates(){
  for(int i = 0; i < 256; i++){
    keystates[i] = false;
  }
}

void respawn() {
  game.resetFade();
  player.create(gameMap.getStartX(), gameMap.getStartY(), gameMap.getStartZ());
}

void draw() {
  game.setWindow(window);
  game.setHeightWidth(height, width);
  game.draw();
  // Swap buffers
  window.swapBuffer();
}

void mouse_pressed(SDL_MouseButtonEvent event) {
  game.mousePressed(event.button);
}

void key_down(SDL_KeyboardEvent event) {
  game.setKey(event.keysym);
}


void key_up(SDL_KeyboardEvent event) {
  game.unsetKey(event.keysym);
}


void resize(int w, int h) {
  window.setSize(w, h);
}

