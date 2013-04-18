#include "Main.hpp"
#include "Box.hpp"
#include "Resources.hpp"



int main(int argc, char **argv) {
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
  game.draw();

  // Draw 2D overlay
  drawOverlay();

  // Swap buffers
  glutSwapBuffers();
}


/**
 * Draws all 2D overlay related.
 * Should be called after drawing all 3D.
 */
void drawOverlay() {
  // Switch to orthographic 2D projection
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  gluOrtho2D(0,width,height,0);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glDisable(GL_DEPTH_TEST);

  // If game is paused
  if(paused) {
    // Add dark tint to screen
    glColor4f(0.f, 0.f, 0.f, 0.5f);
    glDisable(GL_TEXTURE_2D);
    glBegin(GL_QUADS);
    glVertex2f(  0.f,    0.f);
    glVertex2f(  0.f, height);
    glVertex2f(width, height);
    glVertex2f(width,    0.f);
    glEnd();
    // Draw "Paused" message
    glEnable(GL_TEXTURE_2D);
    glColor4f(1.f, 1.f, 1.f, 1.f);
    Resources::inst().bindTexture(TID_STRINGS);
    glBegin(GL_QUADS);
    glTexCoord2f(0.00f, 0.125f); glVertex2f(width/2-64, height/2-32);
    glTexCoord2f(0.00f, 0.250f); glVertex2f(width/2-64, height/2+32);
    glTexCoord2f(0.25f, 0.250f); glVertex2f(width/2+64, height/2+32);
    glTexCoord2f(0.25f, 0.125f); glVertex2f(width/2+64, height/2-32);
    glEnd();
    // If game is not paused
  } else {
    // Draw crosshair if player is alive
    if(player.getState() == PS_ALIVE) {
      Resources::inst().bindTexture(TID_CROSSHAIR);
      glColor4f(1.f, 1.f, 1.f, 1.f);
      glBegin(GL_QUADS);
      glTexCoord2f(0,0); glVertex2f(width/2-16, height/2-12);
      glTexCoord2f(0,1); glVertex2f(width/2-16, height/2+20);
      glTexCoord2f(1,1); glVertex2f(width/2+16, height/2+20);
      glTexCoord2f(1,0); glVertex2f(width/2+16, height/2-12);
      glEnd();
    }
    // Fade screen if player has died or won
    else if(player.getState() != PS_ALIVE) {
      glColor4f(0.f, 0.f, 0.f, fade);
      glDisable(GL_TEXTURE_2D);
      glBegin(GL_QUADS);
      glVertex2f(  0.f,    0.f);
      glVertex2f(  0.f, height);
      glVertex2f(width, height);
      glVertex2f(width,    0.f);
      glEnd();
      glEnable(GL_TEXTURE_2D);
      glColor3f(1.f, 1.f, 1.f);

      // Draw "Press return to respawn" message if dead
      if(player.getState() == PS_DYING) {
	Resources::inst().bindTexture(TID_STRINGS);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0.000f); glVertex2f(width/2-256, height/2-32);
	glTexCoord2f(0, 0.125f); glVertex2f(width/2-256, height/2+32);
	glTexCoord2f(1, 0.125f); glVertex2f(width/2+256, height/2+32);
	glTexCoord2f(1, 0.000f); glVertex2f(width/2+256, height/2-32);
	glEnd();
      }
      // Draw "Press return to continue" message if won
      else if(player.getState() == PS_WON) {
	Resources::inst().bindTexture(TID_STRINGS);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0.25f); glVertex2f(width/2-256, height/2-64);
	glTexCoord2f(0, 0.50f); glVertex2f(width/2-256, height/2+64);
	glTexCoord2f(1, 0.50f); glVertex2f(width/2+256, height/2+64);
	glTexCoord2f(1, 0.25f); glVertex2f(width/2+256, height/2-64);
	glEnd();
      }
    }
  }

  glEnable(GL_DEPTH_TEST);
  // Restore perspective projection matrix
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glMatrixMode(GL_MODELVIEW);
}

/**
 * Called when mouse cursor moves.
 * Saves mouse position two variables mousex and mousey.
 *
 * @param x New X-coordinate
 * @param y New Y-coordinate
 */
void mouse_moved(int x, int y) {
  mousex = x;
  mousey = y;
}

/**
 * Called when a mouse button is pressed.
 * Passes the call to the mousePressed callback in Player
 *
 * @param button The pressed mouse button
 * @param state Mouse buttons stat (e.g. GLUT_DOWN)
 * @param x Cursor's X-coordinate when button was pressed
 * @param y Cursor's Y-coordinate when button was pressed
 */
void mouse_pressed(int button, int state, int x, int y) {
  if(state == GLUT_DOWN) {
    player.mousePressed(button);
  }
}

/**
 * Called when a keyboard button is pressed down.
 * Sets the buttons state to pressed in the keystates array.
 *
 * @param key ASCII value of the key
 * @param x Cursor's X-coordinate when button was pressed
 * @param y Cursor's Y-coordinate when button was pressed
 */
void key_down(unsigned char key, int x, int y) {
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
}

/**
 * Called when a keyboard button is pressed down.
 * Sets the buttons state to not pressed in the keystates array.
 *
 * @param key ASCII value of the key
 * @param x Cursor's X-coordinate when button was pressed
 * @param y Cursor's Y-coordinate when button was pressed
 */
void key_up(unsigned char key, int x, int y) {
  keystates[key] = false;
}

/**
 * Called when the window is resized.
 * Updates the OpenGL viewport and projection matrix.
 *
 * @param w New window width
 * @param h New window height
 */
void resize(int w, int h) {
  // Setup viewport
  glViewport(0, 0, (GLsizei)w, (GLsizei)h);
  // Setup projection matrix
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(60.0, (GLdouble)w/(GLdouble)h, 0.1f, 50.f);

  width = w;
  height = h;

  glMatrixMode(GL_MODELVIEW);
}

/**
 * Called when the window changes status.
 * Used for pausing game when losing focus.
 *
 * @param state Window's new state.
 */
void window_status(int state) {
  if(state != GLUT_VISIBLE) {
    paused = true;
  }
}

/**
 * Sets up the OpenGL context.
 * Loads texture, sets up callbacks and enables lighting, blending etc.
 *
 * @param argc Reference to argument count from main
 * @param argv Arguments from main
 */
void setup(int *argc, char **argv) {
  // Initialize GLUT window
  glutInit(argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_STENCIL);
  glutInitWindowSize(DEFAULT_WIDTH, DEFAULT_HEIGHT);
  glutCreateWindow("glPortal");
  glutSetCursor(GLUT_CURSOR_NONE);
  resize(DEFAULT_WIDTH, DEFAULT_HEIGHT);
  // Initialize GLEW
  glewInit();

  // Reset keystates
  for(int i = 0; i < 256; i++) keystates[i] = false;

  // Setup callback functions
  glutDisplayFunc(draw);
  glutReshapeFunc(resize);
  glutTimerFunc(FRAMETIME, update, 1);
  glutKeyboardFunc(key_down);
  glutKeyboardUpFunc(key_up);
  glutPassiveMotionFunc(mouse_moved);
  glutMouseFunc(mouse_pressed);
  glutWindowStatusFunc(window_status);
  glutWarpPointer(width/2, height/2); // Center pointer

  // Enable textures
  glEnable(GL_TEXTURE_2D);
  // Enable depth testing
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  // Enable back face culling
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  // Set ambient and diffuse lighting
  GLfloat light_DiffAndAmb[4] = {1.f, 1.f, 1.f, 1.f};
  glLightfv(GL_LIGHT0, GL_AMBIENT, light_DiffAndAmb);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, light_DiffAndAmb);

  // Set blending function for portals
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_BLEND);

  // Load textures from files
  Resources::inst().loadTextures();
  Resources::inst().compileShaders();
  Resources::inst().compileModels();

  paused = false;
  nmap_enabled = true;
  current_level = 0;	
  nextLevel();
}
