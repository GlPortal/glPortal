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

  registerCallbacks();
  window.enableGlFeatures();

  game.loadTextures();

  game.unpause();
  nmap_enabled = true;
  game.setPlayer(player);
  game.respawn();
}

void update(int value) {
  if(!game.isPaused()) {
    int centerHorizontal = width/2;
    int centerVertical = height/2;
    float mouseDistanceFromCenterX = static_cast<float>(centerHorizontal-mousex);
    float mouseDistanceFromCenterY = static_cast<float>(centerVertical-mousey);
    glutWarpPointer(centerHorizontal, centerVertical);
    game.update();
    
    if(player.isDead()) {
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

void draw() {
  game.setWindow(window);
  game.setHeightWidth(height, width);
  game.draw();
  // Swap buffers
  glutSwapBuffers();
}


void mouse_moved(int x, int y) {
  game.setMouseCoordinates(x, y);
  mousex = x;
  mousey = y;
}


void mouse_pressed(int button, int state, int x, int y) {
  if(state == GLUT_DOWN) {
    game.mousePressed(button);
  }
}

void key_down(unsigned char key, int x, int y) {
  game.setKey(key);
}


void key_up(unsigned char key, int x, int y) {
  game.unsetKey(key);
}


void resize(int w, int h) {
  window.setSize(w, h);
}

void window_status(int state) {
  if(state != GLUT_VISIBLE) {
    game.pause();
  }
}

