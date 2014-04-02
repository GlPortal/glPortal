#include "Game.hpp"
#include "engine/gui/GameScreen.hpp"
#include "engine/Resources.hpp"
#include "engine/Box.hpp"
#include "engine/renderer/GameMapRenderer.hpp"
#include "util/ListFileParser.hpp"
#include <btBulletDynamicsCommon.h>
#include <stdexcept>

using namespace glPortal::engine;
using namespace glPortal::util;
using namespace glPortal::engine::object;
using namespace glPortal::engine::gui;
using namespace glPortal::engine::renderer;

bool Game::DEBUG = false;

Game::Game(){
  btBroadphaseInterface* broadphase = new btDbvtBroadphase();
  btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();
  btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);
  btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;
  dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher,broadphase,solver,collisionConfiguration);
  dynamicsWorld->setGravity(btVector3(0,-10,0));

  mapRenderer = new GameMapRenderer(std::addressof(gameMap));
  config = Environment::getConfigPointer();
  //Load the configuration settings
  try{
    this->sensitivity = config->getFloatByKey("sensitivity");
  } catch (const std::invalid_argument& e){
    this->sensitivity = 0.005f;
  }
  
  Timer* gameTimer = new Timer();
  this->timer = *gameTimer;
  this->timer.start();
  ListFileParser* listParser = new ListFileParser();
  mapList = listParser->getListFromFile("data/maps/levels.lst");
  this->nextLevel();
}

Game::~Game(){
  delete dynamicsWorld;
}

/**
 * Respawns the player after dying.
 */
void Game::respawn() {
  Vector3f start = gameMap.getStartPosition();
  player.create(start.x, start.y, start.z);
}

void Game::update() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  float dt = Environment::FRAME_TIME/1000;
  
  //Get delta mouse movement
  int mousedx, mousedy;
  SDL_GetRelativeMouseState(&mousedx, &mousedy);
  
  // Apply mouse movement to view
  player.rotation->x -= mousedy * sensitivity;
  player.rotation->y -= mousedx * sensitivity;
  
  // Restrict rotation in horizontal axis
  if(player.rotation->x < -1.5f) player.rotation->x = -1.5f;
  if(player.rotation->x >  1.5f) player.rotation->x =  1.5f;
  
  if(!player.isDead() && !player.hasWon()) {
    // Reset x and z speed
    player.velocity->x = 0;
    player.velocity->z = 0;
    
    // Apply gravity to yspeed
    player.velocity->y -= GRAVITY*dt;
    if(player.velocity->y < -MAXSPEED) player.velocity->y = -MAXSPEED;
    if(player.velocity->y > MAXSPEED) player.velocity->y = MAXSPEED;

    if(keystates['w']) player.moveForward();
    if(keystates['s']) player.moveBackward();
    if(keystates['a']) player.moveLeft();
    if(keystates['d']) player.moveRight();

    if(keystates[' '] && (player.isOnGround() || gameMap.jetpackIsEnabled())) {
      if(player.isOnGround()) {
        player.velocity->y = JUMPPOWER;
      }
    }

    if(keystates[225]) {
      if(gameMap.jetpackIsEnabled()) {
        player.velocity->y += JETPACK_ACC*dt;
      }
    }
    
    // Disable portals if R is held
    if(keystates['r']) {
      portals[0].disable();
      portals[1].disable();
    }
    
    float newx = player.position->x + player.velocity->x * dt;
    float newy = player.position->y + player.velocity->y * dt;
    float newz = player.position->z + player.velocity->z * dt;

    Box bbox;
    bbox.set(newx-0.5, player.position->y, player.position->z-0.5, newx+0.5, player.position->y+1.8, player.position->z+0.5);
    if(gameMap.collidesWithWall(bbox) == false || portals[0].inPortal(bbox) || portals[1].inPortal(bbox)) {
      player.position->x = newx;
    }
    // Check for collision in y-axis
    bbox.set(player.position->x-0.5, player.position->y, newz-0.5, player.position->x+0.5, player.position->y+1.8, newz+0.5);
    if(gameMap.collidesWithWall(bbox) == false || portals[0].inPortal(bbox) || portals[1].inPortal(bbox)) {
      player.position->z = newz;
    }
    // Check for collision in z-axis
    bbox.set(player.position->x-0.5, newy, player.position->z-0.5, player.position->x+0.5, newy+1.8, player.position->z+0.5);
    player.setOffGround();
    if(gameMap.collidesWithWall(bbox) == false || portals[0].inPortal(bbox) || portals[1].inPortal(bbox)) {
      player.position->y = newy;
    } else {
      // If player was falling it means must have hit the ground
      if(player.velocity->y < 0) {
	player.setOnGround();
      }
      player.velocity->y = 0;
    }

    // Check if player has fallen into an acid pool
    bbox.set(player.position->x-0.5, player.position->y, player.position->z-0.5, player.position->x+0.5, player.position->y+1.8, player.position->z+0.5);
    if(gameMap.collidesWithAcid(bbox) == true) {
      player.kill();
    }

    // Check if player has fallen into void
    if(player.position->y <= -30) {
      player.kill();
    }

    // Check if player has taken the cake
    if(gameMap.collidesWithCake(bbox) == true) {
      player.setHasWon();
    }

    // Check if player has entered a portal
    if(portalsActive()) {
      for(int i = 0; i < 2; i++) {
	if(portals[i].throughPortal(player.position->x, player.position->y+0.9f,player.position->z)) {
	  // Calculate rotation between portals
	  float rotation = 0.f;
	  rotation += portals[i].getToRotation()*DEGRAD;
	  rotation += portals[(i+1)%2].getFromRotation()*DEGRAD;
	  player.rotation->y += rotation;
	  // Distance from portal to player
	  float xdist = player.position->x - portals[i].position.x;
	  float zdist = player.position->z - portals[i].position.z;
	  // Calculate this distance when rotated
	  float nxdist = xdist*cos(rotation) + zdist*sin(rotation);
	  float nzdist = zdist*cos(rotation) - xdist*sin(rotation);
	  // Move player to destination portal
	  player.position->x = portals[(i+1)%2].position.x + nxdist;
	  player.position->y = player.position->y + portals[(i+1)%2].position.y - portals[i].position.y;
	  player.position->z = portals[(i+1)%2].position.z + nzdist;
	}
      }
    }
  }
  // If player is dying
  else if(player.isDead()) {
    player.position->y -= (0.60f*dt);
  }

  // Update shots and check their collision with walls
  for(int i = 0; i < 2; i++) {
    Shot *shot = &shots[i];
    if(shot->active) {
      shot->update(dt);

      Box sbox;
      if(gameMap.pointInWall(shot->position.x, shot->position.y, shot->position.z, &sbox)) {
	shot->update(-dt); // Reverse time to before collision
	// Collision really should be interpolated instead
	if(sbox.type == TID_WALL) {
	  portals[i].placeOnBox(sbox, shot->position.x, shot->position.y, shot->position.z, gameMap);
	}
	shot->active = false;
      }
    }
  }
}

void Game::nextLevel() {
  for(int i = 0; i < 2; i++) {
    portals[i].disable();
    shots[i].active = false;		
  }

  if(mapList.size() <= currentLevel){
    gameMap.flush();
    gameMap.setIsLastScreen();
  } else {
    MapFileParser parser;
    gameMap = parser.getMapFromFile("data/maps/" + mapList.at(currentLevel) + ".map");
    respawn();
    currentLevel++;
  }
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
  Vector3f position = *player.position;
  Vector3f rotation = *player.rotation;

  switch(button) {
  case SDL_BUTTON_LEFT:
    // Shoot blue portal
    shots[0].shoot(0, position.x, position.y, position.z, rotation.x, rotation.y);
    break;
  case SDL_BUTTON_RIGHT:
    // Shoot orange portal
    shots[1].shoot(1, position.x, position.y, position.z, rotation.x, rotation.y);
    break;
  case SDL_BUTTON_MIDDLE:
    // Disable both portals
    portals[0].disable();
    portals[1].disable();
    break;
  }
}

void Game::setKey(SDL_Keysym keysym) {
  uint16_t mod = keysym.mod;
  uint16_t key = keysym.sym;
  
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
    currentLevel = key - '0' - 1; // Load levelX
    nextLevel();
  }
  else if(key == 'q') {
    window.close();
    exit(1);
  }
}

void Game::unsetKey(SDL_Keysym keysym) {
  uint16_t mod = keysym.mod;
  uint16_t key = keysym.sym;
  
  //If an invalid key was pressed map it to 0
  if(key < 0 || key > KEY_BUFFER) {
    key = 0;
  }
  keystates[key] = false;
}

void Game::draw() {
  // Clear depth buffer but not color buffer.
  // Every pixel is redrawn every frame anyway.
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();
  
  // Draw scene
  setView();

  try{
    if(config->getStringByKey("portals") != "invisible"){
      mapRenderer->renderPortals(getPortals(), player);
    }
  } catch (const std::invalid_argument& e){
    mapRenderer->renderPortals(getPortals(), player);
  }
  
  mapRenderer->render();
  if(portals[0].active) portals[0].drawOutline(PC_BLUE);
  if(portals[1].active) portals[1].drawOutline(PC_ORANGE);
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
  Vector3f position = *player.position;
  Vector3f rotation = *player.rotation;
  glMatrixMode(GL_MODELVIEW);
  glRotatef(-rotation.x * RADDEG, 1,0,0);
  glRotatef(-rotation.y * RADDEG, 0,1,0);
  glTranslatef(-position.x, -(position.y+2.0f), -position.z);
}

bool Game::normalMapIsActive(){
  return this->nmap_enabled;
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
 * Draws both portal shots
 */
void Game::drawShots() {
  if(shots[0].active) shots[0].draw(player.rotation->x, player.rotation->y);
  if(shots[1].active) shots[1].draw(player.rotation->x, player.rotation->y);
}
