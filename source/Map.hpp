#ifndef __MAP_HPP
#define __MAP_HPP

#include <vector>
#include <GL/glut.h>
#include <string>
#include "engine/Box.hpp"

using namespace glPortal::engine;

class Portal; // Forward declaration

/**
 * Class describing a single map.
 * Contains data about walls and acid pools as well as lighting.
 * Used for performing collision and drawing the current level.
 */
class GameMap {
public:
  void setLightPosition(GLfloat (&position)[4]);
  void setSpawnPosition(float (&position)[3]);
  void setCakePosition(float (&position)[3]);
  void setCakeBox(Box box);
  void setWallVector(std::vector<Box> walls);
  void setAcidVector(std::vector<Box> acid);
  GLfloat* getLightPostition();
  float* getSpawnPosition();
  float* getCakePosition();
  std::vector<Box> getWallVector();
  std::vector<Box> getAcidVector();
  void addWallBox(Box box);
  void addAcidBox(Box box);
  void flush();
  /** Following Methods do not belong here
    * and will get moved out of this class.
    *    
    * Methods to move */
  void draw(bool nmap);
  void drawFromPortal(const Portal& portal, bool nmap);
  bool collidesWithWall(Box &bbox);
  bool collidesWithAcid(Box &bbox);
  bool collidesWithCake(Box &bbox);
  bool pointInWall(float x, float y, float z, Box *box);
  float getStartX() { return startpos[0]; }
  float getStartY() { return startpos[1]; }
  float getStartZ() { return startpos[2]; }
protected:
  void drawBox(Box &b);
  void drawCake();
  void drawLamp();
  // Methods to move end
  std::vector<Box> walls;	/**< Vector of all walls in the map */
  std::vector<Box> acid;	/**< Vector of all acid pools in the map */
  GLfloat lightpos[4];	        /**< Position of the light in the map */
  float startpos[3];		/**< Position where the player spawns in the map */
  float cakepos[3];		/**< Position of the cake in the map */
  Box cakeBox;			/**< Bounding box for collision with cake */
};

#endif
