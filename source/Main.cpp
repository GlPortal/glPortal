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
  glutMainLoop();

  return EXIT_SUCCESS;
}

void setup(int *argc, char **argv) {
  height = window.getHeight();
  width = window.getWidth();

  resetKeyStates();

  registerCallbacks();
  window.enableGlFeatures();

  game.loadTextures();

  game.unpause();
  nmap_enabled = true;
  current_level = 0;	
  nextLevel();
}

void update(int value) {
  if(!game.isPaused()) {
    int centerHorizontal = width/2;
    int centerVertical = height/2;
    float mouseDistanceFromCenterX = static_cast<float>(centerHorizontal-mousex);
    float mouseDistanceFromCenterY = static_cast<float>(centerVertical-mousey);
    glutWarpPointer(centerHorizontal, centerVertical);
    player.update(FRAMETIME_SECONDS, keystates, mouseDistanceFromCenterX, mouseDistanceFromCenterY, map);

    if(player.getState() != PS_ALIVE) {
      game.fadeOut();
    }
  }
  // Redraw screen
  glutPostRedisplay();
  // Schedule new update
  glutTimerFunc(FRAMETIME, update, 1);
}

void resetKeyStates(){
  for(int i = 0; i < 256; i++){
    keystates[i] = false;
  }
}

void registerCallbacks(){
  // Setup callback functions
  glutDisplayFunc(draw);
  glutReshapeFunc(resize);
  glutTimerFunc(FRAMETIME, update, 1);
  glutKeyboardFunc(key_down);
  glutKeyboardUpFunc(key_up);
  glutPassiveMotionFunc(mouse_moved);
  glutMouseFunc(mouse_pressed);
  glutWindowStatusFunc(window_status);
}

void respawn() {
  game.resetFade();
  player.create(map.getStartX(), map.getStartY(), map.getStartZ());
}

void nextLevel() {
  current_level++;
  char filename[] = "data/maps/X.map";
  filename[10] = '0'+current_level; // Hackish but avoids using strings
  MapFileParser parser;
  map = parser.getMapFromFile(filename);
  respawn();
}

void draw() {
  game.setPlayerMap(player, map);
  game.setHeightWidth(height, width);
  game.setNmapEnabled(nmap_enabled);
  game.draw();
  // Swap buffers
  glutSwapBuffers();
}


void mouse_moved(int x, int y) {
  mousex = x;
  mousey = y;
}


void mouse_pressed(int button, int state, int x, int y) {
  if(state == GLUT_DOWN) {
    player.mousePressed(button);
  }
}

void key_down(unsigned char key, int x, int y) {
  game.setKey(key);
  keystates[key] = true;

  if(key == 27) { // Escape key
    game.togglePause();
    glutWarpPointer(width/2, height/2);
  }
  else if(key == 13) { // Return key
    if(player.getState() == PS_DYING) {
      respawn();
    }
    else if(player.getState() == PS_WON) {
      nextLevel();
    }
  }
  else if(key == 'b') {
    nmap_enabled = !nmap_enabled; // Toggle normal mapping
  }
  else if(key >= '0' && key <= '9') {
    current_level = key - '0' - 1; // Load levelX
    nextLevel();
  }
  else if(key == 'q') {
    exit(1);
  }
}


void key_up(unsigned char key, int x, int y) {
  game.setKey(key);
  keystates[key] = false;
}


void resize(int w, int h) {
  window.setSize(w, h);
}

void window_status(int state) {
  if(state != GLUT_VISIBLE) {
    game.pause();
  }
}

