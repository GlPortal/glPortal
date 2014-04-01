#ifndef __MAP_HPP
#define __MAP_HPP

#include <vector>
#include <string>
#include "engine/Box.hpp"
#include "engine/object/Model.hpp"
#include "util/Vector3f.hpp"

using namespace glPortal::engine::object;

using namespace glPortal::engine;
using namespace util;

class Portal; // Forward declaration

/**
 * Class describing a single gameMap.
 * Contains data about walls and acid pools as well as lighting.
 * Used for performing collision and drawing the current level.
 */
class GameMap {
public:
  void setLightPosition(GLfloat (&position)[4]);
  void setBarrelPosition(GLfloat (&position)[4]);
  Vector3f getStartPosition() {return startPos;}
  Vector3f getEndPosition() {return endPos;}
  void setSpawnPosition(float x, float y, float z);
  void setEndPosition(float x, float y, float z);
  void setCakeBox(Box box);
  void setWallVector(std::vector<Box> walls);
  void setAcidVector(std::vector<Box> acid);
  GLfloat* getLightPostition();
  GLfloat* getBarrelPosition();
  float* getSpawnPosition();
  float* getCakePosition();
  std::vector<Box> getWallVector();
  std::vector<Box> getAcidVector();
  void addWallBox(Box box);
  void addAcidBox(Box box);
  void addObject(Model model);
  void flush();
  void enableJetpack();
  bool jetpackIsEnabled();
  /** Following Methods do not belong here
    * and will get moved out of this class.
    *    
    * Methods to move */
  void draw(bool ngameMap);
  void setIsLastScreen();
  bool getIsLastScreen();
  void drawFromPortal(const Portal& portal, bool ngameMap);
  bool collidesWithWall(Box &bbox);
  bool collidesWithAcid(Box &bbox);
  bool collidesWithCake(Box &bbox);
  bool pointInWall(float x, float y, float z, Box *box);
  void renderAvatar(Vector3f position);
  void drawBox(Box &b);
  void drawCake();
  void drawLamp();
  // Methods to move end
  std::vector<Box> walls;	/**< Vector of all walls in the gameMap*/
  std::vector<Box> acid;	/**< Vector of all acid pools in the gameMap*/
  std::vector<Model> objects;
  GLfloat lightpos[4];	        /**< Position of the light in the gameMap*/
  GLfloat barrelPosition[4];
  Vector3f startPos;		/**< Position where the player spawns in the gameMap*/
  Vector3f endPos;	    /**< Position of the goal in the gameMap*/
  Box cakeBox;			/**< Bounding box for collision with cake */

protected:
private:
  bool jetpackEnabled = false;
  bool isLastScreen = false;
};

#endif
