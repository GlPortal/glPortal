#include "Map.hpp"
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <cstdlib>
#include "engine/Resources.hpp"
#include "engine/StringConverter.hpp"
#include "Exception.hpp"
#include "Portal.hpp"


void Map::setLightPosition(GLfloat (&position)[4]){
     this->lightpos[0] = position[0];
     this->lightpos[1] = position[1];
     this->lightpos[2] = position[2];
     this->lightpos[3] = position[3];
     this->lightpos[4] = position[4];
}

GLfloat* Map::getLightPostition(){
  return this->lightpos;
}
void Map::setSpawnPosition(float (&position)[3]){
  this->startpos[0] = position[0];
  this->startpos[1] = position[1];
  this->startpos[2] = position[2];
  this->startpos[3] = position[3];
}

float* Map::getSpawnPosition(){
  return this->startpos;
}

void Map::setCakePosition(float (&position)[3]){
  this->cakepos[0] = position[0];
  this->cakepos[1] = position[1];
  this->cakepos[2] = position[2];
  this->cakepos[3] = position[3];
  this->cakeBox.set(position[0]-0.6f, position[1]-0.6f, position[2]-0.6f,
		    position[0]+0.6f, position[1]+0.2f, position[2]+0.6f);

}

void Map::setCakeBox(Box box){
  this->cakeBox = box;
}

float* Map::getCakePosition(){
  return this->cakepos;
}
void Map::setWallVector(std::vector<Box> walls){}

std::vector<Box> Map::getWallVector(){
  return this->walls;
}

void Map::setAcidVector(std::vector<Box> acid){}

std::vector<Box> Map::getAcidVector(){
  return this->acid;
}
void Map::addWallBox(Box box){
  walls.push_back(box);
}
void Map::addAcidBox(Box box){
  acid.push_back(box);
}

/**
 * Empties the map of all objects
 */
void Map::flush(){
  walls.clear();
  acid.clear();
}

/**
 * Updates light position and draws all maps and acid in level
 *
 * @param nmap True if normal mapping is enabled
 */
void Map::draw(bool nmap) {
  // Update light position
  glLightfv(GL_LIGHT0, GL_POSITION, lightpos);

  // Enable normal mapping
  Resources::inst().enableProgram(nmap ? PID_NMAP : PID_PPL);

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
 * Draws only visible portion of map from a given portal
 *
 * @param portal Portal currently viewing through
 * @param nmap True if normal mapping is enabled
 */
void Map::drawFromPortal(const Portal& portal, bool nmap) {
  // Update light position
  glLightfv(GL_LIGHT0, GL_POSITION, lightpos);

  // Enable normal mapping
  Resources::inst().enableProgram(nmap ? PID_NMAP : PID_PPL);

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
void Map::drawBox(Box &b) {
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
void Map::drawCake() {
  glPushMatrix();
  glTranslatef(cakepos[0], cakepos[1], cakepos[2]);
  Resources::inst().bindTexture(TID_CAKE);
  Resources::inst().drawModel(MID_CAKE);
  glPopMatrix();
}

/**
 * Draws the lamp model at light position
 */
void Map::drawLamp() {
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
 * Checks whether a bounding box collides with any walls in map.
 *
 * @param bbox Bounding box for collision
 *
 * @return True if the box collides
 */
bool Map::collidesWithWall(Box &bbox) {
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
bool Map::collidesWithCake(Box &bbox) {
  return bbox.collide(cakeBox);
}

/**
 * Checks whether a bounding box collides with any acid pool in map.
 *
 * @param bbox Bounding box for collision
 *
 * @return True if the box collides
 */
bool Map::collidesWithAcid(Box &bbox) {
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
bool Map::pointInWall(float x, float y, float z, Box *box = NULL) {
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
