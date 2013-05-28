#include "Main.hpp"
#include "Box.hpp"
#include "Resources.hpp"

int main(int argc, char **argv) {
  window.setup(&argc, argv);
  setup(&argc, argv);
  glutMainLoop();

  return EXIT_SUCCESS;
}

void update(int value) {
  if(!paused) {
    int centerHorizontal = width/2;
    int centerVertical = height/2;
    float mouseDistanceFromCenterX = static_cast<float>(centerHorizontal-mousex);
    float mouseDistanceFromCenterY = static_cast<float>(centerVertical-mousey);
    glutWarpPointer(centerHorizontal, centerVertical);
    player.update(FRAMETIME_SECONDS, keystates, mouseDistanceFromCenterX, mouseDistanceFromCenterY, map);
    // Fade screen if dead/has won
    if(player.getState() != PS_ALIVE) {
      fade += 0.4f*FRAMETIME_SECONDS;
    }
  }

  // Redraw screen
  glutPostRedisplay();
  // Schedule new update
  glutTimerFunc(FRAMETIME, update, 1);
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

void setup(int *argc, char **argv) {
  height = window.getHeight();
  width = window.getWidth();

  // Reset keystates
  for(int i = 0; i < 256; i++) keystates[i] = false;

  registerCallbacks();
  glutWarpPointer(width/2, height/2); // Center pointer

  window.enableGlFeatures();
  // Set ambient and diffuse lighting
  GLfloat light_DiffAndAmb[4] = {1.f, 1.f, 1.f, 1.f};
  glLightfv(GL_LIGHT0, GL_AMBIENT, light_DiffAndAmb);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, light_DiffAndAmb);

  // Set blending function for portals
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_BLEND);

  game.loadTextures();

  paused = false;
  nmap_enabled = true;
  current_level = 0;	
  nextLevel();
}

void respawn() {
  fade = 0.f;
  player.create(map.getStartX(), map.getStartY(), map.getStartZ());
}

void nextLevel() {
  current_level++;
  char filename[] = "maps/X.map";
  filename[5] = '0'+current_level; // Hackish but avoids using strings
  map.load(filename);
  respawn();
}

void draw() {
  game.setPlayerMap(player, map);
  game.setHeightWidth(height, width);
  game.setFade(fade);
  game.setNmapEnabled(nmap_enabled);
  game.draw();

  // Draw 2D overlay
  game.drawOverlay(paused);
 
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
    paused = !paused;
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
    paused = true;
  }
}
