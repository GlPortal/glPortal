#ifndef __MAP_HPP
#define __MAP_HPP

#include <vector>
#include <GL/glut.h>
#include "Box.hpp"
class Portal; // Forward declaration
#include "Portal.hpp"

/**
 * Class describing a single map.
 * Contains data about walls and acid pools as well as lighting.
 * Used for performing collision and drawing the current level.
 */
class Map {
public:
	void load(const char *filename);
	void draw();
	void drawFromPortal(Portal& portal);
	bool collidesWithWall(Box &bbox);
	bool pointInWall(float x, float y, float z, Box *box);

protected:
	void drawBox(Box &b);
	std::vector<Box> *getWalls();
	std::vector<Box> *getAcid();

	std::vector<Box> walls;	/**< Vector of all walls in the map */
	std::vector<Box> acid;	/**< Vector of all acid pools in the map */
	GLfloat lightpos[4];	/**< Position of the position light in the map */
};

#endif
