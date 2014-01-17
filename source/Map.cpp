#include "Map.hpp"
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>
#include "engine/Resources.hpp"
#include "Exception.hpp"
#include "Portal.hpp"

float Map::stringStreamToFloat(std::stringstream& stream){
  std::string string;
  stream >> string;

  return static_cast<float>(atof(string.c_str()));
}

/**
 * Empties the map of all objects
 */
void Map::flushCurrentMap(){
  walls.clear();
  acid.clear();
}

/**
 * Loads map data from a .map file
 *
 * @param filename Path to the .map file
 */
void Map::load(const char *filename) {
  std::ifstream file(filename, std::ifstream::in);
  std::string line, string;

  this->flushCurrentMap();

  // Read map file line by line
  while(file.good()) {
    std::getline(file, line);
    std::stringstream stringStream(line);
    if(line.length() > 0) {
      stringStream >> string;
      float values[6];
      switch(line[0]) {
	// Wall definition
      case 'w':
	for(int i = 0; i < 6; i++) {
	  values[i] = stringStreamToFloat(stringStream);
	}
	walls.push_back(Box(values, TID_WALL));
	break;
	// Tiles definition
      case 't':
	for(int i = 0; i < 6; i++) {
	  values[i] = stringStreamToFloat(stringStream);
	}
	walls.push_back(Box(values, TID_TILES));
	break;
	// Acid pool definition
      case 'a':
	for(int i = 0; i < 6; i++) {
	  values[i] = stringStreamToFloat(stringStream);
	}
	acid.push_back(Box(values, TID_ACID));
	break;
	// Light position
      case 'l':
	for(int i = 0; i < 3; i++) {
	  lightpos[i] = stringStreamToFloat(stringStream);
	}
	lightpos[3] = 1.f; // Set as positioned light
	break;
	// Start position
      case 's':
	for(int i = 0; i < 3; i++) {
	  startpos[i] = stringStreamToFloat(stringStream);
	}
	break;
	// Cake/goal position
      case 'c':
	for(int i = 0; i < 3; i++) {
	  cakepos[i] = stringStreamToFloat(stringStream);
	}
	cakeBox.set(cakepos[0]-0.6f, cakepos[1]-0.6f, cakepos[2]-0.6f,
		    cakepos[0]+0.6f, cakepos[1]+0.2f, cakepos[2]+0.6f);
	break;
      }
    }
  }

  file.close();
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
