#include "Game.hpp"
#include "engine/gui/GameScreen.hpp"
#include "engine/Resources.hpp"
#include "engine/Box.hpp"

using namespace glPortal::engine;
using namespace glPortal::engine::object;
using namespace glPortal::engine::gui;

Game::Game(){
  Timer* gameTimer = new Timer();
  this->timer = *gameTimer;
  //	this->gameTimer = new Timer();
  this->timer.start();
  //Model *model = 
  this->barrel = new Model("data/objects/plastic_barrel/small_barrel.obj");
}

/**
 * Respawns the player after dying.
 */
void Game::respawn() {
  fade = 0.f;
  player.create(gameMap.getStartX(), gameMap.getStartY(), gameMap.getStartZ());
}

/**
 * Loads the next level and respawns the player
 */
void Game::nextLevel() {
  current_level++;
  char filename[] = "data/maps/X.map";
  filename[10] = '0'+current_level; // Hackish but avoids using strings
  MapFileParser parser;
  gameMap = parser.getMapFromFile(filename);
  respawn();
}

// This method is here for refactoring purposes 
// and can be removed once main is decluttered
void Game::setPlayerMap(Player player, GameMap gameMap){
  this->gameMap = gameMap;
  this->player = player;
}

void Game::setPlayer(Player player){
  this->player = player;
}


GameMap Game::getMap(){
  return this->gameMap;
}

Player Game::getPlayer(){
  this->player = player;
}

void Game::setCurrentLevel(int current_level){
  this->current_level = current_level;
}

// This method is here for refactoring purposes 
// and can be removed once main is decluttered
void Game::setHeightWidth(int height, int width){
  this->width = width;
  this->height = height;
}

void Game::setWindow(Window &window){
  this->window = window;
}


void Game::setKey(unsigned char key){
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

void Game::unsetKey(unsigned char key){
  keystates[key] = false;
}

void Game::setFade(float fade){
  this->fade = fade;
}

void Game::resetFade(){
  this->fade = 0.f;
}

void Game::fadeOut(){
  this->fade += 0.4f*FRAMETIME_SECONDS;
}

void Game::draw() {
  // Clear depth buffer but not color buffer.
  // Every pixel is redrawn every frame anyway.
  glClear(GL_DEPTH_BUFFER_BIT);
  // Load identity matrix
  glLoadIdentity();

  // Draw scene
  player.setView();
  drawPortals();
  barrel->draw();
  gameMap.draw(nmap_enabled);
  //gameMap.renderAvatar(player.getX(), player.getY(), player.getZ());

  player.drawPortalOutlines();
  player.drawShots();
  drawOverlay();
}


void Game::setNmapEnabled(bool nmap_enabled){
  this->nmap_enabled = nmap_enabled;
}

/**
 * Draws the inside of both portals as well as their oulines and stencils.
 */
void Game::drawPortals() {
  if(player.portalsActive()) {
    Portal *portals = player.getPortals();
    glEnable(GL_STENCIL_TEST);
    for(int i = 0; i < 2; i++) {
      int src = i;		// Source portal index
      int dst = (i+1)%2;  // Destination portal index

      glPushMatrix();
      // Always write to stencil buffer
      glStencilFunc(GL_NEVER, 1, 0xFF);
      glStencilOp(GL_REPLACE, GL_KEEP, GL_KEEP);
      glStencilMask(0xFF);
      glClear(GL_STENCIL_BUFFER_BIT);

      portals[src].drawStencil();

      glClear(GL_DEPTH_BUFFER_BIT);
      // Only pass stencil test if equal to 1
      glStencilMask(0x00);
      glStencilFunc(GL_EQUAL, 1, 0xFF);

      // Move camera to portal view
      glTranslatef(portals[src].x, portals[src].y, portals[src].z);
      glRotatef(portals[src].getFromRotation(), 0,1,0);
      glRotatef(portals[dst].getToRotation(),   0,1,0);
      glTranslatef(-portals[dst].x, -portals[dst].y, -portals[dst].z);

      // Draw scene from portal view
      gameMap.drawFromPortal(portals[dst], nmap_enabled);
      gameMap.renderAvatar(player.getX(), player.getY(), player.getZ());
      player.drawPortalOutlines();

      glPopMatrix();
    }
    glDisable(GL_STENCIL_TEST);

    // Draw portal stencils so portals wont be drawn over
    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
    glClear(GL_DEPTH_BUFFER_BIT);
    player.drawPortalStencils();
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
  }
}

void Game::loadTextures(){
  Resources::inst().loadTextures();
  Resources::inst().compileShaders();
  Resources::inst().compileModels();
}

/**
 * Draws all 2D overlay related.
 * Should be called after drawing all 3D.
 */
void Game::drawOverlay() {
  GameScreen* screen = new GameScreen(window);
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  gluOrtho2D(0,width,height,0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glDisable(GL_DEPTH_TEST);
  //  screen->drawTestTextScreen();
  // If game is paused
  if(this->isPaused()) {
    screen->drawPauseScreen();
  } else {
    // Draw crosshair if player is alive
    if(player.getState() == PS_ALIVE) {
      screen->drawCrosshair();
    }
    // Fade screen if player has died or won
    else if(player.getState() != PS_ALIVE) {
      screen->drawGameOverScreen();

      // Draw "Press return to respawn" message if dead
      if(player.getState() == PS_DYING) {
	screen->drawRespawnScreen();
      }
      // Draw "Press return to continue" message if won
      else if(player.getState() == PS_WON) {
	screen->drawContinueScreen();
      }
    }
  }
  screen->drawTimer(this->timer.getTimeString());
  glEnable(GL_DEPTH_TEST);
  // Restore perspective projection matrix
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glMatrixMode(GL_MODELVIEW);
}

bool Game::isPaused(){
  return this->paused;
}

void Game::pause(){
  this->paused = true;
}

void Game::unpause(){
  this->paused = false;
}

void Game::togglePause(){
  this->paused = !this->paused;
}
