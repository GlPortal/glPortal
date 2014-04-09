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

void GameMap::setLightPosition(float x, float y, float z) {
  this->light.position.x = x;
  this->light.position.y = y;
  this->light.position.z = z;
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
void GameMap::flush() {
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
