#include "Game.hpp"
#include "engine/gui/GameScreen.hpp"
#include "engine/Resources.hpp"
#include "engine/Box.hpp"

using namespace glPortal::engine;
using namespace glPortal::engine::object;
using namespace glPortal::engine::gui;

bool Game::DEBUG = false;

Game::Game() {
  Timer* gameTimer = new Timer();
  this->timer = *gameTimer;
  this->timer.start();
}

/**
 * Respawns the player after dying.
 */
void Game::respawn() {
  fade = 0.f;
  player.create(gameMap.getStartX(), gameMap.getStartY(), gameMap.getStartZ());
}

void Game::update() {
  float dt = FRAMETIME_SECONDS;
  
  //Get delta mouse movement
  int mousedx, mousedy;
  SDL_GetRelativeMouseState(&mousedx, &mousedy);
  
  // Apply mouse movement to view
  yrot -= mousedx*0.0050f;
  xrot -= mousedy*0.0050f;
  
  // Restrict rotation in horizontal axis
  if(xrot < -1.5f) xrot = -1.5f;
  if(xrot >  1.5f) xrot =  1.5f;
  
  if(!player.isDead() && !player.hasWon()) {
    // Reset x and z speed
    xspeed = zspeed = 0.f;
    // Apply gravity to yspeed
    yspeed -= GRAVITY*dt;
    if(yspeed < -MAXSPEED) yspeed = -MAXSPEED;

    // Move forward
    if(keystates['w']) {
      zspeed -= cos(yrot)*PLAYER_MOVESPEED*dt;
      xspeed -= sin(yrot)*PLAYER_MOVESPEED*dt;
    }
    // Move backward
    if(keystates['s']) {
      zspeed += cos(yrot)*PLAYER_MOVESPEED*dt;
      xspeed += sin(yrot)*PLAYER_MOVESPEED*dt;
    }
    // Strafe left
    if(keystates['a']) {
      xspeed -= cos(yrot)*PLAYER_MOVESPEED*dt;
      zspeed += sin(yrot)*PLAYER_MOVESPEED*dt;
    }
    // Strafe right
    if(keystates['d']) {
      xspeed += cos(yrot)*PLAYER_MOVESPEED*dt;
      zspeed -= sin(yrot)*PLAYER_MOVESPEED*dt;
    }

    if(keystates[' '] && (player.isOnGround() || gameMap.jetpackIsEnabled())) {
      yspeed = JUMPPOWER;
    }

    // Disable portals if R is held
    if(keystates['r']) {
      portals[0].disable();
      portals[1].disable();
    }

    float newx = player.getX() + xspeed*dt;
    float newy = player.getY() + yspeed*dt;
    float newz = player.getZ() + zspeed*dt;

    Box bbox;
    bbox.set(newx-0.5, player.getY(), player.getZ()-0.5, newx+0.5, player.getY()+1.8, player.getZ()+0.5);
    if(gameMap.collidesWithWall(bbox) == false || portals[0].inPortal(bbox) || portals[1].inPortal(bbox)) {
      player.setX(newx);
    }
    // Check for collision in y-axis
    bbox.set(player.getX()-0.5, player.getY(), newz-0.5, player.getX()+0.5, player.getY()+1.8, newz+0.5);
    if(gameMap.collidesWithWall(bbox) == false || portals[0].inPortal(bbox) || portals[1].inPortal(bbox)) {
      player.setZ(newz);
    }
    // Check for collision in z-axis
    bbox.set(player.getX()-0.5, newy, player.getZ()-0.5, player.getX()+0.5, newy+1.8, player.getZ()+0.5);
    player.setOffGround();
    if(gameMap.collidesWithWall(bbox) == false || portals[0].inPortal(bbox) || portals[1].inPortal(bbox)) {
      player.setY(newy);
    } else {
      // If player was falling it means must have hit the ground
      if(yspeed < 0) {
	      player.setOnGround();
      }
      yspeed = 0.f;
    }

    // Check if player has fallen into an acid pool
    bbox.set(player.getX()-0.5, player.getY(), player.getZ()-0.5, player.getX()+0.5, player.getY()+1.8, player.getZ()+0.5);
    if(gameMap.collidesWithAcid(bbox) == true) {
      player.kill();
    }

    // Check if player has taken the cake
    if(gameMap.collidesWithCake(bbox) == true) {
      player.setHasWon();
    }

    // Check if player has entered a portal
    if(portalsActive()) {
      for(int i = 0; i < 2; i++) {
	      if(portals[i].throughPortal(player.getX(), player.getY()+0.9f,player.getZ())) {
	        // Calculate rotation between portals
	        float rotation = 0.f;
	        rotation += portals[i].getToRotation()*DEGRAD;
	        rotation += portals[(i+1)%2].getFromRotation()*DEGRAD;
	        yrot += rotation;
	        // Distance from portal to player
	        float xdist = player.getX() - portals[i].x;
	        float zdist = player.getZ() - portals[i].z;
	        // Calculate this distance when rotated
	        float nxdist = xdist*cos(rotation) + zdist*sin(rotation);
	        float nzdist = zdist*cos(rotation) - xdist*sin(rotation);
	        // Move player to destination portal
	        player.setX(portals[(i+1)%2].x + nxdist);
	        player.setZ(portals[(i+1)%2].z + nzdist);
	        player.setY(player.getY() + portals[(i+1)%2].y - portals[i].y);
	      }
      }
    }
  }
  // If player is dying
  else if(player.isDead()) {
    player.setY(player.getY()-0.60f*dt);
  }

  // Update shots and check their collision with walls
  for(int i = 0; i < 2; i++) {
    if(shots[i].active) {
      shots[i].update(dt);

      Box sbox;
      if(gameMap.pointInWall(shots[i].x, shots[i].y, shots[i].z, &sbox)) {
	      shots[i].update(-dt); // Reverse time to before collision
	      // Collision really should be interpolated instead
	      if(sbox.type == TID_WALL) {
	        portals[i].placeOnBox(sbox, shots[i].x, shots[i].y, shots[i].z, gameMap);
	      }
	      shots[i].active = false;
      }
    }
  }
}

/**
1 * Loads the next level and respawns the player
 */
void Game::nextLevel() {
  for(int i = 0; i < 2; i++) {
    portals[i].disable();
    shots[i].active = false;
		
  }
  current_level++;
  char filename[] = "data/maps/X.map";
  filename[10] = '0'+current_level; // Hackish but avoids using strings
  MapFileParser parser;
  gameMap = parser.getMapFromFile(filename);
  respawn();
}

// This method is here for refactoring purposes 
// and can be removed once main is decluttered
void Game::setPlayerMap(Player &player, GameMap &gameMap){
  this->gameMap = gameMap;
  this->player = player;
}

void Game::setPlayer(Player &player){
  this->player = player;
}


GameMap Game::getMap(){
  return this->gameMap;
}

Player Game::getPlayer(){
  return this->player;
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

/**
 * Called when a mouse button is pressed down
 * @param button Mouse button being pressed
 */
void Game::mousePressed(int button) {
  if(player.isDead()) return;
  float x = player.getX();
  float y = player.getY();
  float z = player.getZ();  

  switch(button) {
  case SDL_BUTTON_LEFT:
    // Shoot blue portal
     shots[0].shoot(0,x,y,z,xrot,yrot);
    break;
  case SDL_BUTTON_RIGHT:
    // Shoot orange portal
    shots[1].shoot(1,x,y,z,xrot,yrot);
    break;
  case SDL_BUTTON_MIDDLE:
    // Disable both portals
     portals[0].disable();
     portals[1].disable();
    break;
  }
}

void Game::setKey(SDL_Keysym keysym) {
  unsigned int mod = keysym.mod;
  unsigned int key = keysym.sym;
  
  //If an invalid key was pressed map it to 0
  if(key < 0 || key > KEY_BUFFER) {
    key = 0;
  }
  
  keystates[key] = true;

  if(key == 27) { // Escape key
    paused = !paused;
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
    window.close();
    exit(1);
  }
}

void Game::unsetKey(SDL_Keysym keysym) {
  unsigned int key = keysym.sym;
  
  //If an invalid key was pressed map it to 0
  if(key < 0 || key > KEY_BUFFER) {
    key = 0;
  }
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
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  // Load identity matrix
  glLoadIdentity();
  
  // Draw scene
  setView();
  drawPortals();
  gameMap.draw(nmap_enabled);
  drawPortalOutlines();
  drawShots();
  drawOverlay();
}


/**
 * Updates the view according to the player's
 *
 * Updates the current MODELVIEW matrix with the inverse
 * translation and rotation of the player's
 */
void Game::setView() {
  glMatrixMode(GL_MODELVIEW);
  glRotatef(-xrot*RADDEG, 1,0,0);
  glRotatef(-yrot*RADDEG, 0,1,0);
  glTranslatef(-player.getX(), -(player.getY()+1.7f), -player.getZ());
}

void Game::setNormalMapActive(bool nmap_enabled){
  this->nmap_enabled = nmap_enabled;
}

bool Game::normalMapIsActive(){
  return this->nmap_enabled;
}

/**
 * Draws the inside of both portals as well as their oulines and stencils.
 */
void Game::drawPortals() {
  if(portalsActive()) {
    Portal *portals = getPortals();
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
      drawPortalOutlines();

      glPopMatrix();
    }
    glDisable(GL_STENCIL_TEST);

    // Draw portal stencils so portals wont be drawn over
    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
    glClear(GL_DEPTH_BUFFER_BIT);
    drawPortalStencils();
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
  screen->beginOverlay();
  
  if(gameMap.getIsLastScreen()){
    screen->drawCreditsScreen();
    screen->endOverlay();
    return;
  }

  if(gameMap.jetpackIsEnabled()){
    screen->drawJetPackEggs();
  } else {
    screen->drawPortalEgg();
  }

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
  screen->endOverlay();
}

bool Game::isPaused() {
  return this->paused;
}

void Game::pause() {
  this->paused = true;
  if(Game::DEBUG == true){
    printf("Game paused");
  }
}

void Game::unpause() {
  this->paused = false;
  if(Game::DEBUG == true){
    printf("Game unpaused\n");
  }
}

void Game::togglePause(){
  this->paused = !this->paused;
}

/**
 * Returns true if both portals are active
 */
bool Game::portalsActive() {
  return (portals[0].active && portals[1].active);
}

/**
 * Draws stencil meshes for both portals
 */
void Game::drawPortalStencils() {
  portals[0].drawStencil();
  portals[1].drawStencil();
}

/**
 * Draws colored outlines for both portals
 */
void Game::drawPortalOutlines() {
  if(portals[0].active) portals[0].drawOutline(PC_BLUE);
  if(portals[1].active) portals[1].drawOutline(PC_ORANGE);
}

/**
 * Draws both portal shots
 */
void Game::drawShots() {
  if(shots[0].active) shots[0].draw(xrot, yrot);
  if(shots[1].active) shots[1].draw(xrot, yrot);
}
