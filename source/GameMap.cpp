#include "GameMap.hpp"
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <cstdlib>
#include "engine/Resources.hpp"
#include "engine/StringConverter.hpp"
#include "Exception.hpp"
#include "Portal.hpp"

void GameMap::setLightPosition(GLfloat (&position)[4]) {
  this->lightpos[0] = position[0];
  this->lightpos[1] = position[1];
  this->lightpos[2] = position[2];
  this->lightpos[3] = position[3];
  this->lightpos[4] = position[4];
}

GLfloat* GameMap::getLightPostition() {
  return this->lightpos;
}

void GameMap::setBarrelPosition(GLfloat (&position)[4]) {
  this->barrelPosition[0] = position[0];
  this->barrelPosition[1] = position[1];
  this->barrelPosition[2] = position[2];
  this->barrelPosition[3] = position[3];
  this->barrelPosition[4] = position[4];
}

GLfloat* GameMap::getBarrelPosition() {
  return this->barrelPosition;
}

void GameMap::setSpawnPosition(float x, float y, float z) {
  this->startPos.x = x;
  this->startPos.y = y;
  this->startPos.z = z;
}

void GameMap::setEndPosition(float x, float y, float z) {
  this->endPos.x = x;
  this->endPos.y = y;
  this->endPos.z = z;
  this->cakeBox.set(x-0.6f, y-0.6f, z-0.6f,
		                x+0.6f, y+0.2f, z+0.6f);
}

void GameMap::setCakeBox(Box box){
  this->cakeBox = box;
}

void GameMap::setWallVector(std::vector<Box> walls){}

std::vector<Box> GameMap::getWallVector(){
  return this->walls;
}

void GameMap::setAcidVector(std::vector<Box> acid){}

std::vector<Box> GameMap::getAcidVector(){
  return this->acid;
}
void GameMap::addWallBox(Box box){
  walls.push_back(box);
}

void GameMap::addAcidBox(Box box){
  acid.push_back(box);
}

void GameMap::addObject(Model model){
  objects.push_back(model);
}


/**
 * Empties the gameMapof all objects
 */
void GameMap::flush(){
  walls.clear();
  acid.clear();
}

void GameMap::enableJetpack(){
  this->jetpackEnabled = true;
}

bool GameMap::jetpackIsEnabled(){
  return this->jetpackEnabled;
}

void GameMap::setIsLastScreen(){
  this->isLastScreen = true;
}

bool GameMap::getIsLastScreen(){
  return this->isLastScreen;
}

/**
 * Updates light position and draws all gameMaps and acid in level
 *
 * @param nMapTrue if normal gameMapping is enabled
 */
void GameMap::draw(bool nMap) {
  // Update light position
  glLightfv(GL_LIGHT0, GL_POSITION, lightpos);

  // Enable normal mapping
  Resources::inst().enableProgram(nMap? PID_NMAP : PID_PPL);
  
  // Draw walls
  TEXTURE_ID current_type = TID_NONE;
  std::vector<Box>::iterator it;
  
  glBegin(GL_QUADS);
  for(it = walls.begin(); it < walls.end(); it++) {
    if(it->type != current_type) {
      glEnd(); // Batches boxes of same material
      current_type = it->type;
      Resources::inst().bindTexture(it->type);
      glBegin(GL_QUADS);
    }
    drawBox(*it);
  }
  glEnd(); 
  
  std::vector<Model>::iterator objectsIterator;
  glBegin(GL_QUADS);
  for(objectsIterator = objects.begin(); objectsIterator < objects.end(); objectsIterator++) {
    objectsIterator->draw();
  }
  glEnd();
	
  // Draw acid waste
  Resources::inst().bindTexture(TID_ACID);
  glBegin(GL_QUADS);
  for(it = acid.begin(); it < acid.end(); it++) {
    drawBox(*it);
  }
  glEnd();

  Resources::inst().disableProgram();

  // Draw cake
  drawCake();

  // Draw lamp
  drawLamp();
}

/**
 * Draws only visible portion of gameMapfrom a given portal
 *
 * @param portal Portal currently viewing through
 * @param nMapTrue if normal gameMapping is enabled
 */
void GameMap::drawFromPortal(const Portal& portal, bool nMap) {
  // Update light position
  glLightfv(GL_LIGHT0, GL_POSITION, lightpos);

  // Enable normal gameMapping
  Resources::inst().enableProgram(nMap? PID_NMAP : PID_PPL);

  // Draw walls
  std::vector<Box>::iterator it;
  TEXTURE_ID current_type = TID_NONE;
  glBegin(GL_QUADS);
  for(it = walls.begin(); it < walls.end(); it++) {
    // Horribly slow bounds check, but smaller code
    if(portal.dir == PD_FRONT && it->z2 > portal.z
       || portal.dir == PD_BACK  && it->z1 < portal.z
       || portal.dir == PD_RIGHT && it->x2 > portal.x
       || portal.dir == PD_LEFT  && it->x1 < portal.x) {
      if(it->type != current_type) {
	      glEnd();
	      current_type = it->type;
	      Resources::inst().bindTexture(it->type);
	      glBegin(GL_QUADS);
      }
      drawBox(*it);
    }
  }
  glEnd();

  // Draw acid waste
  Resources::inst().bindTexture(TID_ACID);
  glBegin(GL_QUADS);
  for(it = acid.begin(); it < acid.end(); it++) {
    if(portal.dir == PD_FRONT && it->z2 > portal.z
       || portal.dir == PD_BACK && it->z1 < portal.z
       || portal.dir == PD_RIGHT && it->x2 > portal.x
       || portal.dir == PD_LEFT && it->x1 < portal.x) {
      drawBox(*it);
    }
  }
  glEnd();

  Resources::inst().disableProgram();

  // Draw cake
  drawCake();

  // Draw lamp
  drawLamp();
}

/**
 * Draws a box
 *
 * @param b Box to draw
 */
void GameMap::drawBox(Box &b) {
  float dx = (b.x2 - b.x1)*0.5f;
  float dy = (b.y2 - b.y1)*0.5f;
  float dz = (b.z2 - b.z1)*0.5f;

  // Top
  glNormal3f(0,1,0);
  glMultiTexCoord4f(GL_TEXTURE1, 1,0,0,-1);
  glTexCoord2f(0.f, 0.f); glVertex3f(b.x1, b.y2, b.z1);
  glTexCoord2f(0.f,  dz); glVertex3f(b.x1, b.y2, b.z2);
  glTexCoord2f( dx,  dz); glVertex3f(b.x2, b.y2, b.z2);
  glTexCoord2f( dx, 0.f); glVertex3f(b.x2, b.y2, b.z1);

  // Bottom
  glNormal3f(0,-1,0);
  glMultiTexCoord4f(GL_TEXTURE1, 1,0,0, -1);
  glTexCoord2f( dx,  dz); glVertex3f(b.x2, b.y1, b.z1);
  glTexCoord2f( dx, 0.f); glVertex3f(b.x2, b.y1, b.z2);
  glTexCoord2f(0.f, 0.f); glVertex3f(b.x1, b.y1, b.z2);
  glTexCoord2f(0.f,  dz); glVertex3f(b.x1, b.y1, b.z1);

  // Front
  glNormal3f(0,0,1);
  glMultiTexCoord4f(GL_TEXTURE1, 1,0,0,-1);
  glTexCoord2f(0.f, 0.f); glVertex3f(b.x1, b.y2, b.z2);
  glTexCoord2f(0.f,  dy); glVertex3f(b.x1, b.y1, b.z2);
  glTexCoord2f( dx,  dy); glVertex3f(b.x2, b.y1, b.z2);
  glTexCoord2f( dx, 0.f); glVertex3f(b.x2, b.y2, b.z2);

  // Back
  glNormal3f(0,0,-1);
  glMultiTexCoord4f(GL_TEXTURE1, -1,0,0, 1);
  glTexCoord2f(0.f,  dy); glVertex3f(b.x2, b.y1, b.z1);
  glTexCoord2f( dx,  dy); glVertex3f(b.x1, b.y1, b.z1);
  glTexCoord2f( dx, 0.f); glVertex3f(b.x1, b.y2, b.z1);
  glTexCoord2f(0.f, 0.f); glVertex3f(b.x2, b.y2, b.z1);

  // Left
  glNormal3f(-1,0,0);
  glMultiTexCoord4f(GL_TEXTURE1, 0,0,1,-1);
  glTexCoord2f(0.f, 0.f); glVertex3f(b.x1, b.y2, b.z1);
  glTexCoord2f(0.f,  dy); glVertex3f(b.x1, b.y1, b.z1);
  glTexCoord2f( dz,  dy); glVertex3f(b.x1, b.y1, b.z2);
  glTexCoord2f( dz, 0.f); glVertex3f(b.x1, b.y2, b.z2);

  // Right
  glNormal3f(1,0,0);
  glMultiTexCoord4f(GL_TEXTURE1, 0,0,-1,-1);
  glTexCoord2f( dz, 0.f); glVertex3f(b.x2, b.y2, b.z1);
  glTexCoord2f(0.f, 0.f); glVertex3f(b.x2, b.y2, b.z2);
  glTexCoord2f(0.f,  dy); glVertex3f(b.x2, b.y1, b.z2);
  glTexCoord2f( dz,  dy); glVertex3f(b.x2, b.y1, b.z1);
}

/**
 * Draws the cake model at cake position
 */
void GameMap::drawCake() {
  glPushMatrix();
  glTranslatef(endPos.x, endPos.y, endPos.z);
  glEnable(GL_TEXTURE_2D);
  Resources::inst().bindTexture(TID_CAKE);
  Resources::inst().drawModel(MID_CAKE);
  glDisable(GL_TEXTURE_2D);
  glPopMatrix();
}

/**
 * Draws the lamp model at light position
 */
void GameMap::drawLamp() {
  glPushMatrix();
  glTranslatef(lightpos[0], lightpos[1], lightpos[2]);
  Box lightBox(-0.8f, 1.f, -0.2f, 0.8f, 0.95f, 0.2f);

  glDisable(GL_TEXTURE_2D);
  glBegin(GL_QUADS);
  drawBox(lightBox);
  glEnd();
  glEnable(GL_TEXTURE_2D);

  glPopMatrix();
}

/**
 * Checks whether a bounding box collides with any walls in gameMap.
 *
 * @param bbox Bounding box for collision
 *
 * @return True if the box collides
 */
bool GameMap::collidesWithWall(Box &bbox) {
  std::vector<Box>::iterator it;
  for(it = walls.begin(); it < walls.end(); it++) {
    if(bbox.collide(*it)) return true;
  }
  return false;
}

/**
 * Checks whether a bounding box collides with the cake.
 *
 * @param bbox Bounding box for collision
 *
 * @return True if the box collides
 */
bool GameMap::collidesWithCake(Box &bbox) {
  return bbox.collide(cakeBox);
}

/**
 * Checks whether a bounding box collides with any acid pool in gameMap.
 *
 * @param bbox Bounding box for collision
 *
 * @return True if the box collides
 */
bool GameMap::collidesWithAcid(Box &bbox) {
  std::vector<Box>::iterator it;
  for(it = acid.begin(); it < acid.end(); it++) {
    if(bbox.collide(*it)) return true;
  }
  return false;
}

/**
 * Checks whether a given point collides with a wall.
 *
 * @param x X-coordinate of the point to collide with
 * @param y Y-coordinate of the point to collide with
 * @param z Z-coordinate of the point to collide with
 * @param box Will point to box point collides with if a collion occurs.
 *
 * @return True if a collision occurs.
 */
bool GameMap::pointInWall(float x, float y, float z, Box *box = NULL) {
  std::vector<Box>::iterator it;
  for(it = walls.begin(); it < walls.end(); it++) {
    if(it->collide(x,y,z)) {
      if(box != NULL) {
	*box = *it;
      }
      return true;
    }
  }
  return false;
}

void GameMap::renderAvatar(Vector3f position){
  float x, y, z;
  x = position.x; y = position.y + 0.7f; z = position.z;
    glBegin(GL_QUADS);        // Draw The Cube Using quads
    glColor3f(0.0f,1.0f,0.0f);    // Color Blue
    glVertex3f(x + 0.3f, y + 0.7f, z - 0.3f);    // Top Right Of The Quad (Top)
    glVertex3f(x - 0.3f, y + 0.7f, z - 0.3f);    // Top Left Of The Quad (Top)
    glVertex3f(x - 0.3f, y + 0.7f, z + 0.3f);    // Bottom Left Of The Quad (Top)
    glVertex3f(x + 0.3f, y + 0.7f, z + 0.3f);    // Bottom Right Of The Quad (Top)
    glColor3f(1.0f,0.5f,0.0f);    // Color Orange
    glVertex3f(x + 0.3f, y - 0.7f, z + 0.3f);    // Top Right Of The Quad (Bottom)
    glVertex3f(x - 0.3f, y - 0.7f, z + 0.3f);    // Top Left Of The Quad (Bottom)
    glVertex3f(x - 0.3f, y - 0.7f, z - 0.3f);    // Bottom Left Of The Quad (Bottom)
    glVertex3f(x + 0.3f, y - 0.7f, z - 0.3f);    // Bottom Right Of The Quad (Bottom)
    glColor3f(1.0f,0.0f,0.0f);    // Color Red    
    glVertex3f(x + 0.3f, y + 0.7f, z + 0.3f);    // Top Right Of The Quad (Front)
    glVertex3f(x - 0.3f, y + 0.7f, z + 0.3f);    // Top Left Of The Quad (Front)
    glVertex3f(x - 0.3f, y - 0.7f, z + 0.3f);    // Bottom Left Of The Quad (Front)
    glVertex3f(x + 0.3f, y - 0.7f, z + 0.3f);    // Bottom Right Of The Quad (Front)
    glColor3f(1.0f,1.0f,0.0f);    // Color Yellow
    glVertex3f(x + 0.3f, y - 0.7f, z - 0.3f);    // Top Right Of The Quad (Back)
    glVertex3f(x - 0.3f, y - 0.7f, z - 0.3f);    // Top Left Of The Quad (Back)
    glVertex3f(x - 0.3f, y + 0.7f, z - 0.3f);    // Bottom Left Of The Quad (Back)
    glVertex3f(x + 0.3f, y + 0.7f, z - 0.3f);    // Bottom Right Of The Quad (Back)
    glColor3f(0.0f,0.0f,1.0f);    // Color Blue
    glVertex3f(x - 0.3f, y + 0.7f, z + 0.3f);    // Top Right Of The Quad (Left)
    glVertex3f(x - 0.3f, y + 0.7f, z - 0.3f);    // Top Left Of The Quad (Left)
    glVertex3f(x - 0.3f, y - 0.7f, z - 0.3f);    // Bottom Left Of The Quad (Left)
    glVertex3f(x - 0.3f, y - 0.7f, z + 0.3f);    // Bottom Right Of The Quad (Left)
    glColor3f(1.0f,0.0f,1.0f);    // Color Violet
    glVertex3f(x + 0.3f, y + 0.7f, z - 0.3f);    // Top Right Of The Quad (Right)
    glVertex3f(x + 0.3f, y + 0.7f, z + 0.3f);    // Top Left Of The Quad (Right)
    glVertex3f(x + 0.3f, y - 0.7f, z + 0.3f);    // Bottom Left Of The Quad (Right)
    glVertex3f(x + 0.3f, y - 0.7f, z - 0.3f);    // Bottom Right Of The Quad (Right)
    glColor3f(1.0f,1.0f,1.0f);    // Color Violet
    glEnd();            // End Drawing The Cube
    glFlush();
  }
